#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "common.h"
#include "Output.h"

enum class CONNECTION_TYPE {
  MOTOR,
  CHUTE,
  LED,
  POWER,
};

typedef struct {
  CONNECTION_TYPE type;
  bool high;
  unsigned long index;
  uint8_t mode;
} pinmapping;

class Motor {
  string name;
  string interpolation;
  bool activated;
  int64_t start_time;
  vector<pair<double, double>> thrust_curve; // <time, force>

public:
  void activate();
  double getForce();
  Motor(string motor_file, string motor_name);
};

class Chute {
  string name;
  bool activated;
  double drag;

public:
  void activate();
  double getDrag();
  Chute(double drag, string name);
};

class LED {
  string name;
  bool activated;

public:
  void set(bool val);
  bool val();
  LED(string name);
};

class Microcontroller {
public:
  string name;
  int id;
  map<int, pinmapping> pin_map;

  Microcontroller(string name, int id);
};

class Rocket {
public:
  string section_name;
  double rocket_weight;
  double rocket_drag;
  vec rocket_pos;  // In meters
  vec rocket_vel;  // In meters / sec
  vec rocket_acc;  // In meters / sec^2. Recalculated every tick, used mainly for logging
  vec rocket_dir;  // = rocket_speed / |rocket_speed|?
                   // At the moment yes, but if the simulator gets more advanced then this will not always be the case. -Leo
  vector<Motor> motors;
  vector<Chute> chutes;
  vector<LED> leds;
  vector<shared_ptr<Microcontroller>> microcontrollers;

  Accelerometer* acc;
  Altimeter* alt;

  void mapPin(string mapping, bool high, unsigned long val, uint8_t mode, CONNECTION_TYPE ty);
  double getDrag();
  Rocket(json rocket_json);
};

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
  vector<vector<shared_ptr<Rocket>>> rocket_sections;

  Environment(string sim_file);
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

#endif
