#pragma once
#include "common.h"
#include "motor.h"
#include "microcontroller.h"
#include "chute.h"
#include "led.h"
#include "silserial.h"
#include "includes/sensors.h"

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
  vector<SILSerial> serials;
  vector<shared_ptr<Microcontroller>> microcontrollers;

  Accelerometer* acc;
  Altimeter* alt;

  void mapPin(string mapping, bool high, unsigned long val, uint8_t mode, CONNECTION_TYPE ty);
  double getDrag();
  Rocket(json rocket_json);
};
