#pragma once
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include <ostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <random>
#include <stdint.h>

#include "config.h"
#include "lib/nlohmann/json.hpp" 

using namespace std;
using json = nlohmann::json;

#define INPUT ((uint8_t)'I')
#define OUTPUT ((uint8_t)'O')
#define INPUT_PULLUP ((uint8_t)'P')
#define PIN_UNDEFINED ((uint8_t)'U')  // for initial state of the pin
#define LOW ((uint8_t)0)
#define HIGH ((uint8_t)1)

struct vec {
  double x;
  double y;
  double z;

  vec();
  vec(double x, double y, double z);

  vec operator+(const vec& b);
  vec operator-(const vec& b);
  vec operator*(const double& b);
  vec operator/(const double& b);
  friend ostream& operator<<(ostream& os, vec v);

  vec norm();
  double mag();
};

void ERROR() __attribute__((__noreturn__));
void ERROR(string msg) __attribute__((__noreturn__));
