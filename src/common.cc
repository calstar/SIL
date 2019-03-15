#include "common.h"

vec::vec() : vec(0, 0, 0) {}

vec::vec(double x, double y, double z) : x(x), y(y), z(z) {}

vec vec::operator+(const vec& b) {
  return vec{this->x + b.x, this->y + b.y, this->z + b.z};
}

vec vec::operator-(const vec& b) {
  return vec{this->x - b.x, this->y - b.y, this->z - b.z};
}

vec vec::operator*(const double& b) {
  return vec{this->x * b, this->y * b, this->z * b};
}

vec vec::operator/(const double& b) {
  return vec{this->x / b, this->y / b, this->z / b};
}

vec vec::norm() {
  double m = this->mag();
  if (m == 0) {
    return *this;
  }
  return *this / m;
}

double vec::mag() {
  return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

ostream& operator<<(ostream& os, vec& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}

void ERROR() {
  ERROR("Unknown error");
}

void ERROR(string msg) {
  cerr << "SIL Failed: " << msg << endl;
  exit(1);
}