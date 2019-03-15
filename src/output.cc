#include "output.h"

Output::Output(json config) {
  file = config["file"].get<string>();
  frequency = config["frequency"].get<int>();
  lastPoll = 0;
  string ty = config["type"].get<string>();
  if (ty == "acceleration") {
    type = OUTPUTTYPE::ACCELERATION;
  } else if (ty == "acceleration") {
    type = OUTPUTTYPE::ACCELERATION;
  } else if (ty == "accelerometer") {
    type = OUTPUTTYPE::ACCELEROMETER;
  } else if (ty == "altitude") {
    type = OUTPUTTYPE::ALTITUDE;
  } else if (ty == "altimeter") {
    type = OUTPUTTYPE::ALTIMETER;
  } else {
    assert(false);
  }
}

void Output::update(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections) {
  if (time >= lastPoll + frequency) {
    lastPoll = time;

    csv << time;
    csv << ", ";

    switch (type) {
    case OUTPUTTYPE::ACCELERATION:
      {
        for (auto& sect : rocket_sections) {
          for (auto& rp : sect) {
            vec acc = rp->rocket_acc;
            csv << rp->section_name;
            csv << ", ";
            csv << acc.x;
            csv << ", ";
            csv << acc.y;
            csv << ", ";
            csv << acc.z;
            csv << ", ";
          }
        }
      }
      break;
    case OUTPUTTYPE::ACCELEROMETER:
      {
        for (auto& sect : rocket_sections) {
          for (auto& rp : sect) {
            vec acc = rp->acc->getData();
            csv << rp->section_name;
            csv << ", ";
            csv << acc.x;
            csv << ", ";
            csv << acc.y;
            csv << ", ";
            csv << acc.z;
            csv << ", ";
          }
        }
      }
      break;
    case OUTPUTTYPE::ALTITUDE:
      {
        for (auto& sect : rocket_sections) {
          for (auto& rp : sect) {
            csv << rp->section_name;
            csv << ", ";
            csv << rp->rocket_pos.z;
            csv << ", ";
          }
        }
      }
      break;
    default:
      assert(false);
    };

    csv << "\n";
  }
}

void Output::finish() {
  while (ifstream(file).good()) file = "_" + file; // Ensure unique filename
  ofstream out(file);
  out << csv.str();
  out.close();
}
