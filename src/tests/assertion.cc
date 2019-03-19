#include "tests/assertion.h"

Assertion::Assertion(json config) {
    string op;
    string v1;
    string v2;
    stringstream str(config.get<string>());
    
    assertionString = config.get<string>();

    str >> v1;
    str >> op;
    str >> v2;

    val1 = setValue(v1);
    val2 = setValue(v2);

    if (op == "=") {
        compareFunction = [](double a, double b) { return a == b; };
    } else if (op == ">") {
        compareFunction = [](double a, double b) { return a > b; };
    } else if (op == "<") {
        compareFunction = [](double a, double b) { return a < b; };
    } else {
        ERROR("Invalid operator for assertion");
    }
}

double* Assertion::setValue(string config) {
    // https://stackoverflow.com/a/29169469
    double d;
    istringstream i(config);
    i >> d >> ws;

    if (!i.fail() && i.eof()) {
        return new double(d); // Memory leak but doesn't really matter
    } else {
        if (config == "max_altitude") {
            return &Environment::global_env->max_altitude;
        } else if (config == "max_acceleration") {
            return &Environment::global_env->max_acceleration;
        } else if (config == "max_speed") {
            return &Environment::global_env->max_speed;
        } else if (config == "landing_speed") {
            return &Environment::global_env->landing_speed;
        } else {
            ERROR("Invalid variable for assertion");
        }
    }
}

bool Assertion::checkAssertion() {
    bool pass = compareFunction(*val1, *val2);
    if (pass) {
        DEBUG_OUT << "Assertion passed: ";
    } else {
        DEBUG_OUT << "Assertion failed: ";
    }
    DEBUG_OUT << assertionString << " (" << *val1 << ", " << *val2 << ")" << endl;
    return pass;
}