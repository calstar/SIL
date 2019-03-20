#include "common.h"
#include "tests/failures/failure.h"

class FailureParser {
private:
    static map<string, function<shared_ptr<Failure>(json)>> typeMap;

public:
    static vector<shared_ptr<Failure>> parseFailures(json config);
};