#ifndef OUTPUT_H
#define OUTPUT_H

#include "common.h"
#include "nlohmann/json.hpp"
#include <sstream>

class Environment;

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

#endif
