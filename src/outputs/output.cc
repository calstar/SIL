#include "outputs/output.h"

Output::Output(json config) {
  file = config["file"].get<string>();
  frequency = config["frequency"].get<int>();
  lastPoll = 0;
}

void Output::update(int64_t time, vector<set<shared_ptr<Rocket>>>& rocket_sections) {
  if (time >= lastPoll + frequency) {
    lastPoll = time;
    log(time, rocket_sections);
  }
}

void Output::finish() {
  while (ifstream(file).good()) file = "_" + file; // Ensure unique filename
  ofstream out(file);
  out << csv.str();
  out.close();
}