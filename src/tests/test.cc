#include "tests/test.h"

Test::Test(json config) {
    name = config["name"].get<string>();

    for (auto& a : config["assertions"]) {
        assertions.emplace_back(a);
    }

    for (auto& f : config["failures"]) {
        failures.emplace_back(f);
    }
}

bool Test::checkAssertions() {
    bool valid = true;
    for (auto& a : assertions) {
        if (!a.checkAssertion()) {
            valid = false;
        }
    } 
    return valid;
}