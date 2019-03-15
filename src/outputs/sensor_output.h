#pragma once
#include "common.h"
#include "outputs/output.h"

class SensorOutput : public Output {
public:
    SensorOutput(json config);
    void log(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections);
};