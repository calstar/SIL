#pragma once
#include "common.h"
#include <string>
#include <vector>

using namespace std;

class SILSerial {
  vector<char> buffer;

public:
  bool sil_input;
  bool sil_output;

  SILSerial();

  void add(string s);
  void add(const char* buf, int len);
  char getc();
  bool empty();
};
