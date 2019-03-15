#include "rocket.h"

void Rocket::mapPin(string mapping, shared_ptr<PinComponent> component) {
  for (auto mcu : microcontrollers) {
    if (mapping.substr(0, mapping.find_first_of(":")) == mcu->name) {
      int index = stoi(mapping.substr(mapping.find_first_of(":") + 1));
      mcu->mapPin(index, {component, false});
      return;
    }
  }
  ERROR("Mapping unknown pin: " + mapping);
}

Rocket::Rocket(json rocket_json) {
  // Get properties of rocket section
  assert(rocket_json.count("weight") == 1);
  rocket_weight = rocket_json["weight"].get<double>();

  assert(rocket_json.count("drag_area") == 1);
  rocket_drag = rocket_json["drag_area"].get<double>();

  assert(rocket_json.count("name") == 1);
  section_name = rocket_json["name"].get<string>();

  // Add microcontrollers
  for (auto& it : rocket_json["microcontrollers"].items()) {
      microcontrollers.push_back(make_shared<Microcontroller>(it.key(), it.value()["id"].get<int>()));
  }

  // Add all motors
  for (auto& it : rocket_json["motors"].items()) {
      motors.emplace_back(make_shared<Motor>(it.value()["file"], it.key()));
      mapPin(it.value()["pin"], motors.back());
  }
  if (motors.size() == 0) {
    DEBUG_OUT << "WARNING: No motors in section: " << section_name << endl;
  }

  // Add all chutes
  for (auto& it : rocket_json["chutes"].items()) {
      chutes.emplace_back(make_shared<Chute>(it.value()["drag_area"], it.key()));
      mapPin(it.value()["pin"], chutes.back());
  }
  if (chutes.size() == 0) {
    DEBUG_OUT << "WARNING: No chutes in section: " << section_name << endl;
  }

  // Add all LEDs
  for (auto& it : rocket_json["leds"].items()) {
      leds.emplace_back(make_shared<LED>(it.key()));
      mapPin(it.value()["pin"], leds.back());
  }
  if (leds.size() == 0) {
    DEBUG_OUT << "WARNING: No LEDs in section: " << section_name << endl;
  }

  // Add sensors
  acc = new Accelerometer(this); // Allocated once per section so memory leak is negligible
  alt = new Altimeter(this); // Allocated once per section so memory leak is negligible
}

double Rocket::getDrag() {
  double ret = rocket_drag;

  for (auto& chute : chutes) {
    ret += chute->getDrag();
  }

  return ret;
}

double Rocket::getForce(int64_t time) {
  double ret = 0;
  for (auto& m : motors) {
    ret += m->getForce(time);
  }
}