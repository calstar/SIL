#ifdef SIL
#include "Harness.h"
namespace code1 {
#else
#include "mbed.h"
#endif

#define LED1 3

DigitalOut myled(LED1);

void loop() {
  // myled = 1;
  // wait(0.2);
  // myled = 0;
  // wait(0.2);
}

#ifdef SIL
} // Close namespace
#else
int main() {
  while(1) {
    loop();
  }
}
#endif
