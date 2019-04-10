#pragma once
#include "common.h"
#include "components/environment.h" 
#include "tests/assertion.h"
#include "tests/failures/failure.h"

class Test {
public:
    string name;
    vector<Failure> failures;
    vector<Assertion> assertions;

    Test(json config);
    bool checkAssertions();
};