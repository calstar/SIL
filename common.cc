#include "common.h"
#include <iostream>

vec operator+(const vec& a, const vec& b) {
  return vec{a.x + b.x, a.y + b.y, a.z + b.z};
}

vec operator-(const vec& a, const vec& b) {
  return vec{a.x - b.x, a.y - b.y, a.z - b.z};
}

vec operator*(const vec& a, const double& b) {
  return vec{a.x * b, a.y * b, a.z * b};
}

vec operator/(const vec& a, const double& b) {
  return vec{a.x / b, a.y / b, a.z / b};
}

vec norm(vec v) {
  double m = mag(v);
  if (m == 0) {
    return v;
  }
  return v / m;
}

double mag(vec v) {
  return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

ostream& operator<<(ostream& os, const vec& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}

_Noreturn void ERROR() {
  ERROR("Unknown error");
}

_Noreturn void ERROR(string msg) {
  cerr << "SIL Failed: " << msg << endl;
  exit(1);
  __builtin_unreachable();
}
