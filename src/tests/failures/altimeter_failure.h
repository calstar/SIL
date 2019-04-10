#pragma once
#include "tests/failures/failure.h"

class AltimeterFailure : public Failure {
public:
    AltimeterFailure(json config);
    void start();
    void loop();
};