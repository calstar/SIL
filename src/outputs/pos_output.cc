#include "outputs/pos_output.h"

PosOutput::PosOutput(json config, string test_name) : Output(config, test_name) {}

void PosOutput::log(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections) {
    for (auto& sect : rocket_sections) {
        for (auto& rp : sect) {
            csv << time << "," << rp->section_name << ",";
            csv << rp->pos.z << ",";
            vec acc = rp->acc;
            csv << acc.x << "," << acc.y << "," << acc.z << ",";
            csv << rp->measured_pos.z << ",";
            vec measured_acc = rp->measured_acc;
            csv << measured_acc.x << "," << measured_acc.y << "," << measured_acc.z;
            csv << endl;
        }
    }
}