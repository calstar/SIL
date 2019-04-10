#pragma once
#include "common.h"
#include "components/rocket.h"

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
  Output(json config, string test_name);
  void update(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections);
  void finish();

};