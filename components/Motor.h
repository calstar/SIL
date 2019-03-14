#pragma once
#include <string>
#include <vector>

using namespace std;

class Motor {
  string name;
  string interpolation;
  bool activated;
  int64_t start_time;
  vector<pair<double, double>> thrust_curve; // <time, force>

public:
  void activate();
  double getForce();
  Motor(string motor_file, string motor_name);
};
