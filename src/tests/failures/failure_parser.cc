#include "tests/failures/failure_parser.h"

// Add more includes here for other failure classes
#include "tests/failures/altimeter_failure.h"

map<string, function<shared_ptr<Failure>(json)>> FailureParser::typeMap = {
    {"altimeter", [](json config) { return make_shared<AltimeterFailure>(config); }}
};

vector<shared_ptr<Failure>> FailureParser::parseFailures(json config) {
    vector<shared_ptr<Failure>> failures;
    for (auto& f : config.items()) {
        string type = f.value()["type"];
        if (typeMap.find(type) == typeMap.end()) {
            ERROR("Invalid config 'type' field");
        } else {
            failures.push_back(typeMap[type](f.value()));
        }
    }
    return failures;
}