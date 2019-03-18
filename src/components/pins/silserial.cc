#include "silserial.h"
#include "components/environment.h"
#include <unistd.h>

using namespace std;

extern int SERIAL_IN_FDS[MCU_LIMIT];
extern int SERIAL_OUT_FDS[MCU_LIMIT];

SILSerial::SILSerial() : PinComponent("Serial") {
  mode = SERIAL_MODE::NORMAL;
}

void SILSerial::add(string s) {
  add(s.c_str(), s.size() + 1);
}

void SILSerial::add(const char* buf, int len) {
  int curr_id = Environment::global_env->current_mcu->id;
  if (mode == SERIAL_MODE::SIL_OUTPUT) {
    int fd = SERIAL_OUT_FDS[curr_id];
    if (fd != 0) {
      write(fd, buf, len);
    } else {
      PRINT_OUT << "Serial Output: " << string(buf, len) << endl;
    }
  } else if (mode == SERIAL_MODE::RADIO_OUTPUT) {
    for (auto section : Environment::global_env->rocket_sections) {
      for (auto roc : section) {
        for (auto serial : roc->serials) {
          if (serial->mode == SERIAL_MODE::RADIO_INPUT) {
            if (serial.get() != this) {
              serial->add(buf, len);
            }
          }
        }
      }
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
