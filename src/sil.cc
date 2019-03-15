#include "common.h"
#include "components/environment.h"
#include "components/rocket.h"
#include "includes/mbed.h"

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

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "Invalid arguments: " << string(argv[0]) << " [sim_file.json]" << endl;
    ERROR("Invalid args");
  }

  DEBUG_OUT << "Loading environment..." << endl;
  Environment env{string(argv[1])};
  Environment::setGlobalEnv(&env);
  DEBUG_OUT << "Environment loaded" << endl;

  vector<bool> code_started(MCU_LIMIT, false);
  vector<int64_t> code_time(MCU_LIMIT, 0); // Time spent in rocket code in microseconds

  // start_timer();
  // code_init();
  // code_time += elapsed();

  DEBUG_OUT << "Starting Simulation" << endl;
  while (!env.done()) {
    bool ran_code = false;
    for (const auto sect : env.rocket_sections) {
      for (auto& rp : sect) {
        for (auto& mcu : rp->microcontrollers) {
          Environment::global_env->current_mcu = mcu;
          Environment::global_env->current_rocket = rp;
          int id = mcu->id;
          if (code_time[id] / CLOCK_MULTIPLIER < env.micros()) {
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
  }
  env.finishOutputs();
  DEBUG_OUT << "Finished Simulation" << endl;
  env.summary();
  Environment::setGlobalEnv(nullptr);
}