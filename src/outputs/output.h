#pragma once
#include "common.h"
#include "components/rocket.h"
#include "includes/sensors.h"


/*
 * Base class for custom output formats
 * Override the log method in implementations
 */ 
class Output {
protected:
  string file;
  int frequency;
  uint64_t lastPoll;
  ostringstream csv;

  virtual void log(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections) = 0;

public:
  Output(json config);
  void update(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections);
  void finish();

};