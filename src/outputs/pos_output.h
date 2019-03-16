#pragma once
#include "common.h"
#include "outputs/output.h"

class PosOutput : public Output {
public:
    PosOutput(json config, string test_name);
    void log(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections);
};