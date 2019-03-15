#pragma once
#include "common.h"

class Noise {
private:
    static default_random_engine engine;
    static normal_distribution<double> normal;

public:

    static double awgn(double stddev);
};