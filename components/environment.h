#pragma once
#include "components/rocket.h"
#include "common.h"
#include "output.h"

class Environment {
  vec wind;       // In meters / sec
  int64_t time;   // In microseconds
  double groundHeight; // In meters
  bool landed;
  map<int, string> mcu_map;

  vector<Output> outputs;

  double max_altitude;
  double max_acceleration;
  double max_speed;

public:
  static Environment* global_env;
  static int current_mcu;

  vector<vector<shared_ptr<Rocket>>> rocket_sections;

  Environment(string sim_file);
  static shared_ptr<Rocket> curr_roc();
  static void setGlobalEnv(Environment* env);
  static void setCurrentMcu(int mcu);
  bool done();
  void tick();
  int64_t micros();
  void setPin(int mcu_id, int pin, bool high);
  int getPin(int mcu_id, int pin);
  void pinMode(int mcu_id, int pin, uint8_t mode);
  void updateOutputs();
  void finishOutputs();
  void summary();
};