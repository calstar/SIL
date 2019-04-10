#include "common.h"
#include "outputs/output.h"

class OutputParser {
private:
    static map<string, function<shared_ptr<Output>(json, string)>> typeMap;

public:
    static vector<shared_ptr<Output>> parseOutputs(json config, string test_name);
};