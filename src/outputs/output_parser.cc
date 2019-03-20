#include "outputs/output_parser.h"

// Add more includes for custom outputs here
#include "outputs/pos_output.h"

map<string, function<shared_ptr<Output>(json, string)>> OutputParser::typeMap = {
    {"pos", [](json config, string name) { return make_shared<PosOutput>(config, name); }}
};

vector<shared_ptr<Output>> OutputParser::parseOutputs(json config, string test_name) {
    vector<shared_ptr<Output>> outputs;
    for (auto& o : config.items()) {
        string type = o.value()["type"];
        if (typeMap.find(type) == typeMap.end()) {
            ERROR("Invalid config 'type' field");
        } else {
            outputs.push_back(typeMap[type](o.value(), test_name));
        }
    }
    return outputs;
}