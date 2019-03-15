#pragma once
#include "components/rocket.h"
#include "common.h"
#include "output.h"

class Environment {
  vec wind;       // In meters / sec
  int64_t time;   // In microseconds
  double groundHeight; // In meters
  bool landed;

  vector<Output> outputs;

  double max_altitude;
  double max_acceleration;
  double max_speed;

public:
  // A list of sets of connected rocket components
  vector<set<shared_ptr<Rocket>>> rocket_sections;
  shared_ptr<Rocket> current_rocket;
  shared_ptr<Microcontroller> current_mcu;

  static void setGlobalEnv(Environment* env);
  static Environment* global_env;

  Environment(string sim_file);
  bool done();
  void tick();
  int64_t micros();
  void updateOutputs();
  void finishOutputs();
  void summary();
};