#pragma once
#include <string>
#include <map>
#include "silserial.h"

using namespace std;

enum class CONNECTION_TYPE {
  MOTOR,
  CHUTE,
  LED,
  POWER,
  SERIAL_TX,
  SERIAL_RX,
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
  bool powered;
  SILSerial* serial_in = NULL;

  Microcontroller(string name, int id);
};
