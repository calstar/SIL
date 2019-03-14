#pragma once
#include <string>

using namespace std;

class Chute {
  string name;
  bool activated;
  double drag;

public:
  void activate();
  double getDrag();
  Chute(double drag, string name);
};
