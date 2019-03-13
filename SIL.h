#include "common.h"
#include "Simulator.h"
#include "Harness.h"

extern Environment* global_env;
extern int current_mcu;

shared_ptr<Rocket> curr_roc();

// class MCU {
//   int ind;
//   uint64_t time_spent;
//   void (*func)();
// };
