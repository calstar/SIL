#pragma once
#include "common.h"
#include "components/rocket.h"
#include "includes/sensors.h"
#include "includes/mbed.h"
#include "lib/nlohmann/json.hpp"
#include <sstream>
#include <fstream>

using json = nlohmann::json;

enum class OUTPUTTYPE {
  ACCELERATION,
  ACCELEROMETER,
  ALTITUDE,
  ALTIMETER,
};

class Output {
  string file;
  OUTPUTTYPE type;
  int frequency;
  uint64_t lastPoll;
  ostringstream csv;

public:
  Output(json config);
  void update();
  void finish();
};