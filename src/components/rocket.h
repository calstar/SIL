#pragma once
#include "common.h"
#include "pin_components/motor.h"
#include "components/microcontroller.h"
#include "pin_components/chute.h"
#include "pin_components/led.h"
#include "pin_components/pinComponent.h"
#include "includes/sensors.h"

class Rocket {
private:
  void mapPin(string mapping, shared_ptr<PinComponent> component);

public:
  string section_name;
  double rocket_weight;
  double rocket_drag;
  vec rocket_pos;  // In meters
  vec rocket_vel;  // In meters / sec
  vec rocket_acc;  // In meters / sec^2. Recalculated every tick, used mainly for logging
  vec rocket_dir;  // = rocket_speed / |rocket_speed|?
                   // At the moment yes, but if the simulator gets more advanced then this will not always be the case. -Leo
  vector<shared_ptr<Motor>> motors;
  vector<shared_ptr<Chute>> chutes;
  vector<shared_ptr<LED>> leds;
  vector<shared_ptr<Microcontroller>> microcontrollers;

  Accelerometer* acc;
  Altimeter* alt;

  double getDrag();
  double getForce(int64_t time);
  Rocket(json rocket_json);
};
