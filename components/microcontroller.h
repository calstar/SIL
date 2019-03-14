#pragma once
#include <string>
#include <map>

using namespace std;

enum class CONNECTION_TYPE {
  MOTOR,
  CHUTE,
  LED,
  POWER,
};

typedef struct {
  CONNECTION_TYPE type;
  bool high;
  unsigned long index;
  uint8_t mode;
} pinmapping;

class Microcontroller {
public:
  string name;
  int id;
  map<int, pinmapping> pin_map;

  Microcontroller(string name, int id);
};
