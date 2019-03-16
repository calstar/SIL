#include "motor.h"

Motor::Motor(string motor_file, string motor_name) : PinComponent(motor_name) {
  // open file stream
  ifstream file(motor_file);
  assert(!file.fail());

  // read json from filestream
  json motor_json;
  file >> motor_json;

  // place values into motor
  interpolation = motor_json["interpolation"].get<string>();

  // Load thrust curve. NOTE: Assumes thrust curve is provided in time-increasing order
  for (auto it = motor_json["thrust_curve"].begin(); it != motor_json["thrust_curve"].end(); ++it) {
    double time = stof(it.key());
    double force = it.value();
    thrust_curve.push_back(make_pair(time, force));
  }
}


double Motor::getForce(int64_t current) {
  // Thrust curve must not be empty
  assert(this->thrust_curve.size() > 0);

  // Return 0 if not started
  if (!isActivated) {
    return 0;
  }

  double time = (double)(current - start_time) / 1000000.0;

  // Return 0 if still in fuse
  if (time < 0) {
    return 0;
  }

  // if before first entry of thrust curve, return first thrust value
  if (time <= thrust_curve[0].first) {
    return thrust_curve[0].second;
  }

  // if after last entry, return last thrust value
  if (time >= thrust_curve[thrust_curve.size() - 1].first) {
    return thrust_curve[thrust_curve.size() - 1].second;
  }

  // otherwise, find the first entry of the thrust curve after the given time
  int index = 0;
  while (thrust_curve[index].first < time) {
    index++;
  }

  // If it's on a point then return the point
  // if (abs(thrust_curve[index].first - time) {
  //   return thrust_curve[index].second;
  // }

  // interpolate between index-1 and index
  if (interpolation == "linear") {
    // linear interpolation
    double force_0 = thrust_curve[index - 1].second;
    double force_1 = thrust_curve[index].second;
    double t_1 = thrust_curve[index].first;
    double t_0 = thrust_curve[index - 1].first;
    double slope = (force_1 - force_0) / (t_1 - t_0);
    return force_0 + slope * (time - t_0);
  }

  ERROR("Motor has unknown interpolation");
}

void Motor::activate(int64_t time) {
  if (!isActivated) {
    DEBUG_OUT << "Motor \"" << name << "\" activated" << endl;
    isActivated = true;
    start_time = time;
  }
}
