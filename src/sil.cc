#include "common.h"
#include "components/environment.h"
#include "components/rocket.h"
#include "tests/test.h"
#include "includes/mbed.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/poll.h>

chrono::high_resolution_clock::time_point timer;
void start_timer() {
  timer = chrono::high_resolution_clock::now();
}
int64_t elapsed() {
  auto diff = chrono::high_resolution_clock::now() - timer;
  return chrono::duration_cast<chrono::microseconds>(diff).count();
}

void (*starts[MCU_LIMIT])() = { code0::start, code1::start, code2::start, code3::start, code4::start };
void (*loops[MCU_LIMIT])() = { code0::loop, code1::loop, code2::loop, code3::loop, code4::loop };

int SERIAL_IN_FDS[MCU_LIMIT];
int SERIAL_OUT_FDS[MCU_LIMIT];
bool ENABLE_TERMINAL[MCU_LIMIT] = {false};
#define MAX_BUF 1024

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "Invalid arguments: ./" << string(argv[0]) << "[sim_file.json] (-s01234)" << endl;
    assert(false);
  }

  for (int i = 2; i < argc; i++) {
    string arg = argv[i];
    if (arg.size() < 2) {
      ERROR();
    }
    if (arg.substr(0, 2) == "-s") {
      for (int j = 2; j < arg.size(); j++) {
        ENABLE_TERMINAL[arg[j] - '0'] = true;
      }
    }
  }

  for (int i = 0; i < MCU_LIMIT; i++) {
    if (ENABLE_TERMINAL[i]) {
      DEBUG_OUT << "Enabling terminal " << i << "... (open ./build/terminal " << i << ")" << endl;
#ifdef __linux__
      string tempdir = "/tmp/";
#else
      string tempdir = getenv("TMPDIR");
#endif
      string outpath = tempdir + "SILOUTTERMINAL" + to_string(i);
      string inpath = tempdir + "SILINTERMINAL" + to_string(i);
      remove(outpath.c_str());
      remove(inpath.c_str());
      mkfifo(outpath.c_str(), 0666);
      mkfifo(inpath.c_str(), 0666);
      int outfd = open(outpath.c_str(), O_WRONLY);
      int infd = open(inpath.c_str(), O_RDONLY);
      SERIAL_OUT_FDS[i] = outfd;
      SERIAL_IN_FDS[i] = infd;
      DEBUG_OUT << "Enabled terminal" << endl;
    }
  }

  ifstream file(argv[1]);
  if (!file.good()) {
    cerr << "File does not exist: " << argv[1] << endl;

    assert(false);
  }

  json config;
  file >> config;
  file.close();

  for (auto& test_config : config["tests"]) {
    string test_name = test_config["name"].get<string>();
    DEBUG_OUT << endl << "================================================================" << endl;
    DEBUG_OUT << "Running with test: " << test_name << endl << endl;
    DEBUG_OUT << "Loading environment..." << endl;
    Environment env(config, test_name);
    Environment::setGlobalEnv(&env);
    DEBUG_OUT << "Environment loaded" << endl;

    Test test(test_config); // Has to be after environment setup because pointers

    vector<bool> code_started(MCU_LIMIT, false);
    vector<int64_t> code_time(MCU_LIMIT, 0); // Time spent in rocket code in microseconds


    DEBUG_OUT << "Starting Simulation" << endl;
    while (!env.done()) {
      bool ran_code = false;
      for (const auto sect : env.rocket_sections) {
        for (auto rp : sect) {
          for (auto mcu : rp->microcontrollers) {
            Environment::global_env->current_mcu = mcu;
            Environment::global_env->current_rocket = rp;
            int id = mcu->id;
            if (mcu->powered && code_time[id] / CLOCK_MULTIPLIER < env.micros()) {
              VERBOSE_OUT << "Running code " << id << endl;
              start_timer();
              if (code_started[id]) {
                loops[id]();
              } else {
                starts[id]();
                code_started[id] = true;
              }
              code_time[id] += elapsed() + CODE_OVERHEAD_PENALTY;
              ran_code = true;
            }
          }
        }
      }
      // if (!ran_code) {
        env.tick();
      // } else {
      //   // TODO: Maybe put a warning here for loops that run twice without environment ticking
      // }
      VERBOSE_OUT << env.micros() << endl;
      env.updateOutputs();

      for (int i = 0; i < MCU_LIMIT; i++) {
        if (ENABLE_TERMINAL[i]) {
          char buf[MAX_BUF];
          struct pollfd fds;
          fds.events = POLLIN;
          fds.fd = SERIAL_IN_FDS[i]; /* this is STDIN */
          if (poll(&fds, 1, 0) == 1) {
            int len = read(SERIAL_IN_FDS[i], buf, MAX_BUF);
            if (len > 0) {
              printf("Received: %s\n", buf);
              for (const auto sect : env.rocket_sections) {
                for (auto rp : sect) {
                  for (auto mcu : rp->microcontrollers) {
                    if (mcu->id == i) {
                      if (mcu->serial_in == NULL) ERROR("MCU has no serial in but is receiving serial commands");
                      mcu->serial_in->add(buf, len);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    env.finishOutputs();
    DEBUG_OUT << "Finished Simulation" << endl << endl;;
    env.summary();
    DEBUG_OUT << endl;
    if (test.checkAssertions()) {
      DEBUG_OUT << "Test " << test.name << " PASSED ALL ASSERTIONS!" << endl;
    } else {
      DEBUG_OUT << "Test " << test.name << " FAILED ASSERTION!" << endl;
    }
    DEBUG_OUT << "================================================================" << endl << endl;
    Environment::setGlobalEnv(nullptr);
  }
}
