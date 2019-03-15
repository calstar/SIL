#include "noise.h"

default_random_engine Noise::engine;
normal_distribution<double> Noise::normal;

double Noise::awgn(double stddev) {
    return Noise::normal(Noise::engine) * stddev;
}