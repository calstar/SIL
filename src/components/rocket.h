#pragma once
#include "common.h"
#include "components/pins//motor.h"
#include "components/noise.h"
#include "components/microcontroller.h"
#include "components/pins//silserial.h"
#include "components/pins//chute.h"
#include "components/pins//led.h"
#include "components/pins//pincomponent.h"

class Rocket {
private:
  void mapPin(string mapping, shared_ptr<PinComponent> component);

public:
  string section_name;
  bool grounded;
  double weight;
  double drag;
  vec pos;  // In meters
  vec measured_pos;
  vec vel;  // In meters / sec
  vec acc;  // In meters / sec^2. Recalculated every tick, used mainly for logging
  vec measured_acc;
  vec dir;  // = rocket_speed / |rocket_speed|?
                   // At the moment yes, but if the simulator gets more advanced then this will not always be the case. -Leo
  vector<shared_ptr<Motor>> motors;
  vector<shared_ptr<Chute>> chutes;
  vector<shared_ptr<LED>> leds;
  vector<shared_ptr<SILSerial>> serials;
  vector<shared_ptr<Microcontroller>> microcontrollers;

  double getDrag();
  double getForce(int64_t time);
  void setState(vec pos, vec vel, vec acc, vec dir);
  Rocket(json config);

  static vector<set<shared_ptr<Rocket>>> parseParts(json config, vec init_pos, vec init_vel, vec init_accel, vec init_dir);
};
