#pragma once
#include "common.h"
#include "pincomponent.h"

class LED : public PinComponent {

public:
  LED(string name);
  void activate(int64_t time);
  void deactivate(int64_t time);
};
