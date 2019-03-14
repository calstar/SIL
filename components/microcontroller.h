#pragma once
#include <string>
#include <map>

using namespace std;

class Microcontroller {
public:
  string name;
  int id;
  map<int, pinmapping> pin_map;

  Microcontroller(string name, int id);
};
