#include "environment.h"

Environment* Environment::global_env = nullptr;

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
  outputs = OutputParser::parseOutputs(sim_json["output"]);

  DEBUG_OUT << "Loading rocket" << endl;
  ifstream rocket_file(sim_json["rocket"].get<string>());
  assert(!rocket_file.fail());
  json rocket_json;
  rocket_file >> rocket_json;
  rocket_sections = Rocket::parseParts(rocket_json, vec(0, 0, groundHeight), vec(0, 0, 0), vec(0, 0, -9.81), vec(0, 0, 1));

  max_altitude = 0;
  max_speed = 0;
  max_acceleration = 0;
}

void Environment::setGlobalEnv(Environment* env) {
  Environment::global_env = env;
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

    vec old_vel = (*section.begin())->vel;
    vec old_pos = (*section.begin())->pos;
    vec old_dir = (*section.begin())->dir;

    double weight = 0; // kg
    double drag = 0;
    for (auto roc : section) {
      weight += roc->weight;
      drag = max(drag, roc->getDrag());
    }

    vec effective_wind = wind - old_vel;

    vec wind_force = effective_wind.norm() * (AIR_DENSITY * AIR_DRAG_COEF * drag * pow(effective_wind.mag(), 2) / 2.0);
    force = force + wind_force;

    double motor_force = 0;
    for (auto roc : section) {
      motor_force += roc->getForce(micros());
    }
    force = force + old_dir * motor_force;

    acc = acc + force / weight;

    vec new_vel = old_vel + acc * delta; // m/s
    vec new_pos = old_pos + new_vel * delta;

    if (new_pos.z < groundHeight) {
      new_pos.z = groundHeight;

      if (new_vel.z < -0.1) {
        double speed = new_vel.mag();
        DEBUG_OUT << "Rocket landed at height " << new_pos.z << " at " << speed << " m/s" << endl;
        landed = true;
      }

      new_vel.x = 0;
      new_vel.y = 0;
      new_vel.z = 0;
    }

    for (auto roc : section) {
      roc->setState(new_pos, new_vel, acc, roc->dir);
    }

    max_acceleration = max(acc.mag(), max_acceleration);
    max_speed = max(new_vel.mag(), max_speed);
    max_altitude = max(new_pos.z, max_altitude);

    VERBOSE_OUT << "Time: " << time << "  Rocket pos: " << new_pos << "  Acc: " << acc << endl;
  }

}

int64_t Environment::micros() {
  return time;
}

void Environment::updateOutputs() {
  for (auto out : outputs) {
    out->update(this->micros(), this->rocket_sections);
  }
}

void Environment::finishOutputs() {
  for (auto out : outputs) {
    out->finish();
  }
}

void Environment::summary() {
  DEBUG_OUT << "Max Altitude: " << max_altitude << " meters" << endl;
  DEBUG_OUT << "Max Speed: " << max_speed << " meters/s" << endl;
  DEBUG_OUT << "Max Acceleration: " << max_acceleration << " meters/s^2" << endl;
}
