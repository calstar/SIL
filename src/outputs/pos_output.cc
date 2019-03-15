#include "outputs/pos_output.h"

PosOutput::PosOutput(json config) : Output(config) {}

void PosOutput::log(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections) {
    for (auto& sect : rocket_sections) {
        for (auto& rp : sect) {
            csv << time << "," << rp->section_name << ",";
            csv << rp->pos.z << ",";
            vec acc = rp->acc;
            csv << acc.x << "," << acc.y << "," << acc.z;
            csv << endl;
        }
    }
}