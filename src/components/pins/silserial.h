#pragma once
#include "common.h"
#include "pincomponent.h"
#include <string>
#include <vector>

using namespace std;

class SILSerial : public PinComponent {
  vector<char> buffer;

public:
  bool sil_input;
  bool sil_output;

  SILSerial();

  void add(string s);
  void add(const char* buf, int len);
  char getc();
  bool empty();
  void activate(int64_t time);
  void deactivate(int64_t time);
};
