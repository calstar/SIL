#include "common.h"
#include "outputs/output.h"

class OutputParser {
public:
    static vector<shared_ptr<Output>> parseOutputs(json config, string test_name);
};