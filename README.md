# SIL
Software-in-the-loop simulator for rockets

## Adding output formats
* Create a .cc and .h file for the intended output (subclass of Output)
* Add relevant parsing code for the output in output_parser.cc and .h

## Weird things are happening (segfault, etc.) when running or compiling the program
* Try `make clean` first
