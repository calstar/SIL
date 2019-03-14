// Header for SIL and Mbed
#ifdef SIL
#include "Harness.h"
#define HEADER(ID)  \
namespace code##ID {
#else
#include "mbed.h"
#endif



// Footer for SIL and Mbed
#ifdef SIL
#define FOOTER  \
}
#else
#define FOOTER  \
int main() {    \
  while(1) {    \
    loop();     \
  }             \
}
#endif
