#pragma once
#include "common.h"
#include "components/rocket.h"
#include "outputs/output_parser.h"

class Environment {
  vec wind;       // In meters / sec
  int64_t time;   // In microseconds
  double groundHeight; // In meters
  bool landed;

  vector<shared_ptr<Output>> outputs;

public:
  // A list of sets of connected rocket components
  vector<set<shared_ptr<Rocket>>> rocket_sections;
  shared_ptr<Rocket> current_rocket;
  shared_ptr<Microcontroller> current_mcu;

  double max_altitude;
  double max_acceleration;
  double max_speed;
  double landing_speed;
  int64_t landing_time;

  static void setGlobalEnv(Environment* env);
  static Environment* global_env;

  Environment(json config);
  bool done();
  void tick();
  int64_t micros();
  void updateOutputs();
  void finishOutputs();
  void summary();
};
