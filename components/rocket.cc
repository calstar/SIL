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
  vector<tuple<string, bool, unsigned long, uint8_t, CONNECTION_TYPE>> to_map; // Wait until everything is added to map these
  if (rocket_json.count("microcontrollers") != 0) {
    for (auto it = rocket_json["microcontrollers"].begin(); it != rocket_json["microcontrollers"].end(); ++it) {
      microcontrollers.push_back(make_shared<Microcontroller>(it.key(), it.value()["id"].get<int>()));
      if (it.value().count("pin") != 0) {
        string powerpin = it.value()["pin"].get<string>();
        microcontrollers[microcontrollers.size() - 1]->powered = false;
        to_map.push_back({powerpin, false, microcontrollers.size() - 1, PIN_UNDEFINED, CONNECTION_TYPE::POWER});
      }
    }
  }
  for (auto m : to_map) {
    mapPin(get<0>(m), get<1>(m), get<2>(m), get<3>(m), get<4>(m));
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

  // Connect serial ports
  if (rocket_json.count("communications") != 0) {
    for (auto p : rocket_json["communications"]) {
      serials.emplace_back();
      if (p[0] == "SIL_INPUT") {
        serials[serials.size() - 1].sil_input = true;
        string mapping = p[1];
        bool found = false;
        for (auto mcu : microcontrollers) {
          if (mapping.substr(0, mapping.find_first_of(":")) == mcu->name) {
            mcu->serial_in = &serials[serials.size() - 1];
            found = true;
          }
        }
        if (!found) ERROR("MCU not found");
      } else {
        mapPin(p[0], false, serials.size() - 1, PIN_UNDEFINED, CONNECTION_TYPE::SERIAL_TX);
      }
      if (p[1] == "SIL_OUTPUT") {
        serials[serials.size() - 1].sil_output = true;
      } else {
        mapPin(p[1], false, serials.size() - 1, PIN_UNDEFINED, CONNECTION_TYPE::SERIAL_RX);
      }
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
