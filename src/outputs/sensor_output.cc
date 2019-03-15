#include "outputs/sensor_output.h"

SensorOutput::SensorOutput(json config) : Output(config) {}

void SensorOutput::log(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections) {
    for (auto& sect : rocket_sections) {
        for (auto& rp : sect) {
            csv << time << "," << rp->section_name << ",";
            vec acc = rp->rocket_acc;
            csv << acc.x << "," << acc.y << "," << acc.z << ",";
            vec accSensor = rp->acc->getData();
            csv << accSensor.x << "," << accSensor.y << "," << accSensor.z << ",";
            csv << rp->rocket_pos.z << ",";
            csv << endl;
        }
    }
}