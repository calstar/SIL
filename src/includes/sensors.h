#pragma once
#include "common.h"

class Rocket; // To resolve circular dependency

class Accelerometer {
  Rocket* roc;
  vec currentError;

public:
  Accelerometer(Rocket* roc);
  vec getData();
};

class Altimeter {
  Rocket* roc;
  float currentError;

public:
  Altimeter(Rocket* roc);
  float getData();
};