#pragma once
#include "common.h"
#include "components/rocket.h"
#include "includes/sensors.h"

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
  void update(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections);
  void finish();
};