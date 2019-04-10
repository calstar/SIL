#pragma once
#include "common.h"
#include "components/environment.h"

class Assertion {
private:
    string assertionString;
    double* val1;
    double* val2;
    function<bool(double, double)> compareFunction;

    double* setValue(string config);

public:
    Assertion(json config);
    bool checkAssertion();
};