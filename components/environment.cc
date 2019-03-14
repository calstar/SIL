#include "environment.h"

int Environment::current_mcu = 0;
Environment* Environment::global_env = 0;

Environment::Environment(string sim_file) {
  ifstream file(sim_file);
  if (!file.good()) {
    cerr << "File does not exist: " << sim_file << endl;
    assert(false);
  }

  json sim_json;
  file >> sim_json;
  file.close();

  time = 0;
  groundHeight = sim_json["ground_height"]; // TODO: Make a customizable randomized input
  landed = false;

  // TODO: Make a customizable randomized input
  if (sim_json["wind"]["type"] == "fixed") {
    wind.x = sim_json["wind"]["x"];
    wind.y = sim_json["wind"]["y"];
    wind.z = sim_json["wind"]["z"];
  } else {
    assert(false);
  }

  DEBUG_OUT << "Loading outputs" << endl;
  for (auto out : sim_json["output"]) {
    outputs.emplace_back(out);
  }

  DEBUG_OUT << "Loading rocket" << endl;
  ifstream rocket_file(sim_json["rocket"].get<string>());
  assert(!rocket_file.fail());
  json rocket_json;
  rocket_file >> rocket_json;
  vector<shared_ptr<Rocket>> main_section;

  for (json rocket_section : rocket_json) {
    auto r = make_shared<Rocket>(rocket_section);
    DEBUG_OUT << "Loaded section \"" << r->section_name << "\"" << endl;

    r->rocket_pos.x = 0;
    r->rocket_pos.y = 0;
    r->rocket_pos.z = groundHeight;

    r->rocket_vel.x = 0;
    r->rocket_vel.y = 0;
    r->rocket_vel.z = 0;

    r->rocket_acc.x = 0;
    r->rocket_acc.y = 0;
    r->rocket_acc.z = -9.81;

    r->rocket_dir.x = 0;
    r->rocket_dir.y = 0;
    r->rocket_dir.z = 1;

    main_section.push_back(r);
  }
  rocket_sections.push_back(main_section);
  DEBUG_OUT << "Rocket loaded" << endl;

  max_altitude = 0;
  max_speed = 0;
  max_acceleration = 0;
}

void Environment::setGlobalEnv(Environment* env) {
  Environment::global_env = env;
}

void Environment::setCurrentMcu(int mcu) {
  Environment::current_mcu = mcu;
}

shared_ptr<Rocket> Environment::curr_roc() {
  for (auto& sect : Environment::global_env->rocket_sections) {
    for (auto rp : sect) {
      for (auto mcu : rp->microcontrollers) {
        if (mcu->id == Environment::current_mcu) {
          return rp;
        }
      }
    }
  }
  ERROR();
}

bool Environment::done() {
  // TODO: Use timeout
  return landed;
}

void Environment::tick() {
  // TODO: Tick simulation
  time += TICK_MICROS;
  double delta = TICK_MICROS / 1000000.0;

  for (auto section : rocket_sections) {
    if (section.size() == 0) continue;

    vec acc{0, 0, -9.81}; // m/s
    vec force{0, 0, 0}; // N

    vec old_vel = section[0]->rocket_vel;
    vec old_pos = section[0]->rocket_pos;
    vec old_dir = section[0]->rocket_dir;

    double weight = 0; // kg
    double drag = 0;
    for (auto roc : section) {
      weight += roc->rocket_weight;
      drag = max(drag, roc->getDrag());
    }

    vec effective_wind = wind - old_vel;

    vec wind_force = norm(effective_wind) * (AIR_DENSITY * AIR_DRAG_COEF * drag * pow(mag(effective_wind), 2) / 2.0);
    force = force + wind_force;

    double motor_force = 0;
    for (auto roc : section) {
      for (Motor m : roc->motors) {
        motor_force += m.getForce(this->micros());
      }
    }
    force = force + old_dir * motor_force;

    acc = acc + force / weight;

    vec new_vel = old_vel + acc * delta; // m/s
    vec new_pos = old_pos + new_vel * delta;

    if (new_pos.z < groundHeight) {
      new_pos.z = groundHeight;

      if (new_vel.z < -0.1) {
        double speed = mag(new_vel);
        DEBUG_OUT << "Rocket landed at height " << new_pos.z << " at " << speed << " m/s" << endl;
        landed = true;
      }

      new_vel.x = 0;
      new_vel.y = 0;
      new_vel.z = 0;
    }

    for (auto roc : section) {
      roc->rocket_acc = acc;
      roc->rocket_vel = new_vel;
      roc->rocket_pos = new_pos;
    }

    max_acceleration = max(mag(acc), max_acceleration);
    max_speed = max(mag(new_vel), max_speed);
    max_altitude = max(new_pos.z, max_altitude);

    VERBOSE_OUT << "Time: " << time << "  Rocket pos: " << new_pos << "  Acc: " << acc << endl;
  }

}

int64_t Environment::micros() {
  return time;
}

void Environment::setPin(int mcu_id, int pin, bool high) {
  for (auto section : rocket_sections) {
    for (auto roc : section) {
      for (auto mcu : roc->microcontrollers) {
        if (mcu->id == mcu_id) {

          if (mcu->pin_map.count(pin) == 1) {
            auto& pmap = mcu->pin_map[pin];

            if (pmap.mode == PIN_UNDEFINED) {
              DEBUG_OUT << "Warning: Attempting to write to pin " << pin << " before setting pin mode." << endl;
            } else if (pmap.mode == INPUT || pmap.mode == INPUT_PULLUP) {
              DEBUG_OUT << "Warning: Attempting to write to input pin " << pin << endl;
            }

            pmap.high = high;
            switch (pmap.type) {
              case CONNECTION_TYPE::MOTOR:
              if (high == true) {
                roc->motors.at(pmap.index).activate(this->micros()); // TODO: Maybe set delay on this so super fast writes don't set off the motor
              }
              break;

              case CONNECTION_TYPE::CHUTE:
              if (high == true) {
                roc->chutes.at(pmap.index).activate(); // TODO: Maybe set delay on this so super fast writes don't set off the black powder
              }
              break;

              case CONNECTION_TYPE::LED:
              roc->leds.at(pmap.index).set(high); // TODO: Maybe set delay on this so super fast writes don't set off the black powder
              break;

              default:
              ERROR("Unknown pin type set");
            }
          } else {
            ERROR("setPin() called with when pin not set up");
          }

          return;
        }
      }
    }
  }

  ERROR("setPin() called with invalid mcu_id");
}

int Environment::getPin(int mcu_id, int pin) {
  for (auto section : rocket_sections) {
    for (auto roc : section) {
      for (auto mcu : roc->microcontrollers) {
        if (mcu->id == mcu_id) {
          if (mcu->pin_map.count(pin) == 1) {
            auto& pmap = mcu->pin_map[pin];

            if (pmap.mode == INPUT) {
              return pmap.high;
            } else {
              assert(false && "Invalid pin mode for pin read");
            }
          } else {
            assert(false && "Pin not set up");
          }
        }
      }
    }
  }
  ERROR("getPin() called and pin not found");
}

void Environment::pinMode(int mcu_id, int pin, uint8_t mode) {
  assert(mode == INPUT || mode == OUTPUT || mode == INPUT_PULLUP);

  for (auto section : rocket_sections) {
    for (auto roc : section) {
      for (auto mcu : roc->microcontrollers) {
        if (mcu->id == mcu_id) {
          if (mcu->pin_map.count(pin) == 1) {
            auto& pmap = mcu->pin_map[pin];
            if (pmap.mode != mode) DEBUG_OUT << "Setting pin " << pin << " of mcu " << mcu_id << " to mode " << mode << endl;
            pmap.mode = mode;
            return;
          }
        }
      }
    }
  }
  ERROR("pinMode called on nonexistent pin " + to_string(pin) + " on mcu " + to_string(mcu_id));
}

void Environment::updateOutputs() {
  for (Output& out : outputs) {
    out.update(this->micros(), this->rocket_sections);
  }
}

void Environment::finishOutputs() {
  for (Output& out : outputs) {
    out.finish();
  }
}

void Environment::summary() {
  DEBUG_OUT << "Max Altitude: " << max_altitude << " meters" << endl;
  DEBUG_OUT << "Max Speed: " << max_speed << " meters/s" << endl;
  DEBUG_OUT << "Max Acceleration: " << max_acceleration << " meters/s^2" << endl;
}
