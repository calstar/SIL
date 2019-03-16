#include "silserial.h"
#include "components/environment.h"
#include <unistd.h>

using namespace std;

extern int SERIAL_IN_FDS[MCU_LIMIT];
extern int SERIAL_OUT_FDS[MCU_LIMIT];

SILSerial::SILSerial() : PinComponent("Serial") {
  sil_input = false;
  sil_output = false;
}

void SILSerial::add(string s) {
  add(s.c_str(), s.size() + 1);
}

void SILSerial::add(const char* buf, int len) {
  if (sil_output) {
    int fd = SERIAL_OUT_FDS[Environment::global_env->current_mcu->id];
    if (fd != 0) {
      write(fd, buf, len);
    } else {
      PRINT_OUT << "Serial Output: " << string(buf, len) << endl;
    }
  } else {
    for (int i = 0; i < len; i++) {
      buffer.push_back(buf[i]);
    }
  }
}

char SILSerial::getc() {
  if (empty()) ERROR();
  char ret = buffer.at(0);
  buffer.erase(buffer.begin());
  return ret;
}

bool SILSerial::empty() {
  return buffer.size() == 0;
}

void SILSerial::activate(int64_t time) {
  ERROR();
}

void SILSerial::deactivate(int64_t time) {
  ERROR();
}
