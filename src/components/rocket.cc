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

Rocket::Rocket(json config) {
  // If section is grounded (e.g. ground station)
  grounded = config.count("grounded") == 1 && config["grounded"].get<bool>() == true;

  if (!grounded) {
    // Get properties of rocket section
    assert(config.count("weight") == 1);
    weight = config["weight"].get<double>();

    assert(config.count("drag_area") == 1);
    drag = config["drag_area"].get<double>();
  }

  assert(config.count("name") == 1);
  section_name = config["name"].get<string>();

  // Add microcontrollers
  vector<pair<string, shared_ptr<PinComponent>>> to_map; // Wait until everything is added to map these
  for (auto& it : config["microcontrollers"].items()) {
      shared_ptr<Microcontroller> mcu = make_shared<Microcontroller>(it.key(), it.value()["id"].get<int>());
      microcontrollers.push_back(mcu);
      if (it.value().count("pin") != 0) {
        string powerpin = it.value()["pin"].get<string>();
        microcontrollers.back()->powered = false;
        to_map.push_back(make_pair(powerpin, microcontrollers.back()));
      }
  }
  for (auto m : to_map) {
    mapPin(m.first, m.second);
  }

  if (!grounded) {
    // Add all motors
    for (auto& it : config["motors"].items()) {
        motors.push_back(make_shared<Motor>(it.value()["file"], it.key()));
        if (it.value().find("pin") != it.value().end()) {
            mapPin(it.value()["pin"], motors.back());
        } else if (it.value().find("fuse") != it.value().end()) {
            // Sets fuse on motor to start after simulation starts
            motors.back()->activate((int64_t)(it.value()["fuse"].get<float>() * 1000000));
        } else {
            // Turns on the motor at the start if there's no pin set
            motors.back()->activate(0);
        }
    }
    if (motors.size() == 0) {
      DEBUG_OUT << "WARNING: No motors in section: " << section_name << endl;
    }

    // Add all chutes
    for (auto& it : config["chutes"].items()) {
        chutes.push_back(make_shared<Chute>(it.value()["drag_area"], it.key()));
        mapPin(it.value()["pin"], chutes.back());
    }
    if (chutes.size() == 0) {
      DEBUG_OUT << "WARNING: No chutes in section: " << section_name << endl;
    }
  }

  // Add all LEDs
  for (auto& it : config["leds"].items()) {
      leds.push_back(make_shared<LED>(it.key()));
      mapPin(it.value()["pin"], leds.back());
  }
  if (leds.size() == 0) {
    DEBUG_OUT << "WARNING: No LEDs in section: " << section_name << endl;
  }

  // Connect serial ports
  if (config.count("communications") != 0) {
    for (auto p : config["communications"]) {
      serials.emplace_back(make_shared<SILSerial>());
      if (p[0] == "SIL_INPUT") {
        serials.back()->mode = SERIAL_MODE::SIL_INPUT;
        string mapping = p[1];
        bool found = false;
        for (auto mcu : microcontrollers) {
          if (mapping.substr(0, mapping.find_first_of(":")) == mcu->name) {
            mcu->serial_in = serials.back();
            found = true;
          }
        }
        if (!found) ERROR("MCU not found");
      } else if (p[0] == "RADIO_INPUT") {
        serials.back()->mode = SERIAL_MODE::RADIO_INPUT;
      } else {
        mapPin(p[0], serials.back());
      }
      if (p[1] == "SIL_OUTPUT") {
        serials.back()->mode = SERIAL_MODE::SIL_OUTPUT;
      } else if (p[1] == "RADIO_OUTPUT") {
        serials.back()->mode = SERIAL_MODE::RADIO_OUTPUT;
      } else {
        mapPin(p[1], serials.back());
      }
    }
  }
}

void Rocket::setState(vec pos, vec vel, vec acc, vec dir) {
  if (grounded) return;

  this->pos = pos;
  this->measured_pos = pos + vec(0, 0, Noise::awgn(ALT_NOISE_MAGNITUDE));
  this->vel = vel;
  this->acc = acc;
  this->measured_acc = acc; // TODO
  this->dir = dir;
}

vector<set<shared_ptr<Rocket>>> Rocket::parseParts(json config, vec init_pos, vec init_vel, vec init_accel, vec init_dir) {
  vector<set<shared_ptr<Rocket>>> rocket_sections;
  set<shared_ptr<Rocket>> main_section;
  for (json rocket_section : config) {
    auto r = make_shared<Rocket>(rocket_section);
    DEBUG_OUT << "Loaded section \"" << r->section_name << "\"" << endl;
    r->setState(init_pos, init_vel, init_accel, init_dir);

    if (r->grounded) {
      rocket_sections.push_back({r});
    } else {
      main_section.insert(r);
    }
  }
  rocket_sections.push_back(main_section);
  return rocket_sections;
}

double Rocket::getDrag() {
  if (grounded) return 0;

  double ret = drag;

  for (auto& chute : chutes) {
    ret += chute->getDrag();
  }

  return ret;
}

double Rocket::getForce(int64_t time) {
  if (grounded) return 0;

  double ret = 0;
  for (auto& m : motors) {
    ret += m->getForce(time);
  }
  return ret;
}
