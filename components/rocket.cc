#include "rocket.h"

void Rocket::mapPin(string mapping, bool high, unsigned long val, uint8_t mode, CONNECTION_TYPE ty) {
  for (auto mcu : microcontrollers) {
    if (mapping.substr(0, mapping.find_first_of(":")) == mcu->name) {
      int pin = stoi(mapping.substr(mapping.find_first_of(":") + 1));
      assert(mcu->pin_map.count(pin) == 0);
      mcu->pin_map[pin] = {ty, high, val, mode};
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
  if (rocket_json.count("microcontrollers") != 0) {
    for (auto it = rocket_json["microcontrollers"].begin(); it != rocket_json["microcontrollers"].end(); ++it) {
      microcontrollers.push_back(make_shared<Microcontroller>(it.key(), it.value()["id"].get<int>()));
    }
  }

  // Add all motors
  if (rocket_json.count("motors") != 0) {
    for (auto it = rocket_json["motors"].begin(); it != rocket_json["motors"].end(); ++it) {
      motors.emplace_back(it.value()["file"], it.key());
      mapPin(it.value()["pin"], false, motors.size() - 1, PIN_UNDEFINED, CONNECTION_TYPE::MOTOR);
    }
  }
  if (motors.size() == 0) {
    DEBUG_OUT << "WARNING: No motors in section: " << section_name << endl;
  }

  // Add all chutes
  if (rocket_json.count("chutes") != 0) {
    for (auto it = rocket_json["chutes"].begin(); it != rocket_json["chutes"].end(); ++it) {
      chutes.emplace_back(it.value()["drag_area"], it.key());
      mapPin(it.value()["pin"], false, chutes.size() - 1, PIN_UNDEFINED, CONNECTION_TYPE::CHUTE);
    }
  }
  if (chutes.size() == 0) {
    DEBUG_OUT << "WARNING: No chutes in section: " << section_name << endl;
  }

  // Add all LEDs
  if (rocket_json.count("leds") != 0) {
    for (auto it = rocket_json["leds"].begin(); it != rocket_json["leds"].end(); ++it) {
      leds.emplace_back(it.key());
      mapPin(it.value(), false, leds.size() - 1, PIN_UNDEFINED, CONNECTION_TYPE::LED);
    }
  }

  // Add sensors
  acc = new Accelerometer(this); // Allocated once per section so memory leak is negligible
  alt = new Altimeter(this); // Allocated once per section so memory leak is negligible
}

double Rocket::getDrag() {
  double ret = rocket_drag;

  for (auto chute : chutes) {
    ret += chute.getDrag();
  }

  return ret;
}
