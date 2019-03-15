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
  // Get properties of rocket section
  assert(config.count("weight") == 1);
  weight = config["weight"].get<double>();

  assert(config.count("drag_area") == 1);
  drag = config["drag_area"].get<double>();

  assert(config.count("name") == 1);
  section_name = config["name"].get<string>();

  // Add microcontrollers
  for (auto& it : config["microcontrollers"].items()) {
      microcontrollers.push_back(make_shared<Microcontroller>(it.key(), it.value()["id"].get<int>()));
  }

  // Add all motors
  for (auto& it : config["motors"].items()) {
      motors.emplace_back(make_shared<Motor>(it.value()["file"], it.key()));
      mapPin(it.value()["pin"], motors.back());
  }
  if (motors.size() == 0) {
    DEBUG_OUT << "WARNING: No motors in section: " << section_name << endl;
  }

  // Add all chutes
  for (auto& it : config["chutes"].items()) {
      chutes.emplace_back(make_shared<Chute>(it.value()["drag_area"], it.key()));
      mapPin(it.value()["pin"], chutes.back());
  }
  if (chutes.size() == 0) {
    DEBUG_OUT << "WARNING: No chutes in section: " << section_name << endl;
  }

  // Add all LEDs
  for (auto& it : config["leds"].items()) {
      leds.emplace_back(make_shared<LED>(it.key()));
      mapPin(it.value()["pin"], leds.back());
  }
  if (leds.size() == 0) {
    DEBUG_OUT << "WARNING: No LEDs in section: " << section_name << endl;
  }
}

vector<set<shared_ptr<Rocket>>> Rocket::parseParts(json config, vec init_pos, vec init_vel, vec init_accel, vec init_dir) {
  vector<set<shared_ptr<Rocket>>> rocket_sections;
  set<shared_ptr<Rocket>> main_section;
  for (json rocket_section : config) {
    auto r = make_shared<Rocket>(rocket_section);
    DEBUG_OUT << "Loaded section \"" << r->section_name << "\"" << endl;

    r->pos = init_pos;
    r->vel = init_vel;
    r->acc = init_accel;
    r->dir = init_dir;

    main_section.insert(r);
  }
  rocket_sections.push_back(main_section);
  return rocket_sections;
}

double Rocket::getDrag() {
  double ret = drag;

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
  return ret;
}