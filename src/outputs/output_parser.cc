#include "outputs/output_parser.h"

// Add more includes for custom outputs here
#include "outputs/sensor_output.h"

vector<shared_ptr<Output>> OutputParser::parseOutputs(json config) {
    vector<shared_ptr<Output>> outputs;
    for (auto& o : config.items()) {
        if (o.value()["type"] == "sensor") {
            outputs.push_back(make_shared<SensorOutput>(o.value()));
        } else {
            ERROR("Invalid config 'type' field");
        }
    }
    return outputs;
}