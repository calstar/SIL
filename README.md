# SIL
Software-in-the-loop simulator for rockets

## Required code file format
```c
  // Includes must be at top of file
  #include "mbed.h"
  
  // Start function, run at the beginning of the program
  void start() {
    // Code here
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

## Adding output formats
* Create a .cc and .h file for the intended output (subclass of Output)
* Add relevant parsing code for the output in output_parser.cc and .h

## Weird things are happening (segfault, etc.) when running or compiling the program
* Try `make clean` first
