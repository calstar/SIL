#include "common.h"
#include "components/environment.h"
#include "components/rocket.h"
#include "includes/mbed.h"
#include <iostream>

using namespace std;

chrono::system_clock::time_point timer;
void start_timer() {
  timer = chrono::high_resolution_clock::now();
}
int64_t elapsed() {
  auto diff = chrono::high_resolution_clock::now() - timer;
  return chrono::duration_cast<chrono::microseconds>(diff).count();
}

void (*loops[MCU_LIMIT])() = { code0::loop, code1::loop, code2::loop, code3::loop, code4::loop };

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "Invalid arguments: ./" << string(argv[0]) << " [sim_file.json]" << endl;
    assert(false);
  }

  DEBUG_OUT << "Loading environment..." << endl;
  Environment env{string(argv[1])};
  Environment::setGlobalEnv(&env);
  DEBUG_OUT << "Environment loaded" << endl;

  vector<int64_t> code_time(MCU_LIMIT, 0); // Time spent in rocket code in microseconds

  // start_timer();
  // code_init();
  // code_time += elapsed();

  DEBUG_OUT << "Starting Simulation" << endl;
  while (!env.done()) {
    bool ran_code = false;
    for (const auto sect : env.rocket_sections) {
      for (auto rp : sect) {
        for (auto mcu : rp->microcontrollers) {
          Environment::current_mcu = mcu->id;
          if (code_time[Environment::current_mcu] / CLOCK_MULTIPLIER < env.micros()) {
            VERBOSE_OUT << "Running code " << Environment::current_mcu << endl;
            start_timer();
            loops[Environment::current_mcu]();
            code_time[Environment::current_mcu] += elapsed() + CODE_OVERHEAD_PENALTY;
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
  Environment::setGlobalEnv(NULL);
}
