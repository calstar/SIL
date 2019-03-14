#pragma once
#include <string>
#include <vector>
#include <cassert>
#include <fstream>
#include "common.h" 
#include "lib/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class Motor {
  string name;
  string interpolation;
  bool activated;
  int64_t start_time;
  vector<pair<double, double>> thrust_curve; // <time, force>

public:
  void activate(int64_t current);
  double getForce(int64_t current);
  Motor(string motor_file, string motor_name);
};
