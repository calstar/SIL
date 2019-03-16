#include "outputs/output_parser.h"

// Add more includes for custom outputs here
#include "outputs/pos_output.h"

vector<shared_ptr<Output>> OutputParser::parseOutputs(json config, string test_name) {
    vector<shared_ptr<Output>> outputs;
    for (auto& o : config.items()) {
        if (o.value()["type"] == "pos") {
            outputs.push_back(make_shared<PosOutput>(o.value(), test_name));
        } else {
            ERROR("Invalid config 'type' field");
        }
    }
    return outputs;
}