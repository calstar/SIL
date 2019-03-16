# SIL
Software-in-the-loop simulator for rockets

## Required code file format
```c
  // Includes must be at top of file
  #include "mbed.h"
  // < Other includes here >
  
  // < Defines and global variables should come after the includes and before main >
  
  // Start function, run at the beginning of the program
  void start() {
    // < Code here >
  }
  
  // Loop function, run continuously
  void loop() {
    // Code here
  }

  // Main is optional but must be at the end of the file if it exists
int main() {
  start();
  while (1) {
    loop();
  }
}
```

## Compiling SIL
Run make and pass in microcontroller code:
```
make code0=../launch-firmware/fc/fc.h code1=../launch-firmware/tpc/tpc.h
```

## Running SIL
* To run a simulation file

```./build/sil config/simulations/testsim.json```
* To connect to the debug serial ports, add `-sN`:

```./build/sil config/simulations/testsim.json -s01```
* Then in separate terminal windows, open the serial client for each microcontroller:

```
./build/terminal 0
./build/terminal 1
```

## Adding output formats
* Create a .cc and .h file for the intended output (subclass of Output)
* Add relevant parsing code for the output in output_parser.cc and .h

## Weird things are happening (segfault, etc.) when running or compiling the program
* Try `make clean` first
