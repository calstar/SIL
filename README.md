# SIL
Software-in-the-loop simulator for rockets

## Compiling SIL
SIL works on Mac or Linux. Windows is not currently supported, but you can run it in the Vagrant VM. A C++ compiler (clang++ or g++) and Python (any version) are required. PM Leo or Jaren if compilation fails.

Run make and pass in microcontroller code:
```
make code0=../launch-firmware/fc/fc.h code1=../launch-firmware/tpc/tpc.h
```
### Required microcontroller code file format
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
Code should be self-contained. Any includes except for flatbuffers and mbed will have to be copied to the SIL includes directory.
## Running SIL
* To run a simulation file

```./build/sil config/simulations/testsim.json```
* To connect to the debug serial ports, add `-sN` (unconnected serial ports will print to main output):

```./build/sil config/simulations/testsim.json -s01```
* Then in separate terminal windows, open the serial client for each microcontroller:

```
./build/terminal 0
./build/terminal 1
```

### Simulation file formats
Don't actually use // comments, they're not allowed in json 😊 
#### Simulation file format
```javascript
{
  "rocket": "config/rockets/testrocket.json", // Rocket file
  "wind": { // Simualted wind in environment
    "type": "fixed",  // Only type for now
    "x": 1, // Wind dir vector
    "y": 0,
    "z": 0
  },
  "ground_height": 10,  // Ground height at start
  "ground_height_far": 20,  // Ground height when rocket moves away from start pos
  "timeout": 3, // When to end the sim after no movement
  "output": [ // List of output types
    {
      "type": "pos", // Output type (see list of classes subclassing Output)
      "file": "pos_data.csv", // Output filename
      "frequency": 10000 // Polling frequency
    }
  ],
  "tests": [ // List of tests to run, with various failures and required assertions (separate environment for each)
    {
      "name": "test1",
      "assertions": [ // List of assertions checked at launch finish
        "max_altitude > 300", // Assertion format: {number | environment_variable} [><=] {number | environment_variable}
        "landing_speed < 10"
      ],
      "failures": [ // List of failures
        "altimeter" // Failure name (see failure classes for details)
      ]
    }
  ]
}

```
#### Rocket file format
```javascript
[ // Array of rocket stages
  {
    "name": "booster", // Stage name
    "weight": 5,       // Rocket stage weight
    "drag_area": 0.02, // Rocket stage drag area
    "leds": {          // List of LEDs on rocket stage
      "led1": {        // LED Name
        "pin": "flight_computer:7" // Connected PIN for LED
      }
    },
    "motors": {       // List of motors on rocket stage
      "booster": {    // Motor name
        "file": "config/motors/aerotech_j800t.json", // Motor JSON config file
        "fuse": 10    // Fuse time (when the rocket will be activated after the sim start)
      }
    },
    "separators": {   // List of stage separators
      "stage1": {     // Name
        "separate": "sustainer",  // Stage to separate (doesn't exist in this example)
        "pin": "flight_computer:10" // Connected pin
      }
    },
    "chutes": {       // List of chutes
      "drogue": {     // Name
        "drag_area": 5,   // Chute drag area
        "pin": "flight_computer:12" // Connected pin
      }
    },
    "microcontrollers": { // List of MCUs
      "flight_computer": {  // Name
        "id": 0,  // ID --> should be unique to file
        "pin": "telemetry_power_control:24" // Powering pin (optional, if not present the mcu will always be on)
      },
      "telemetry_power_control": {
        "id": 1
      }
    },
    "communications": [ // List of UART connections
      ["flight_computer:4", "telemetry_power_control:3"],
      ["telemetry_power_control:4", "flight_computer:3"],
      ["SIL_INPUT", "flight_computer:5"], // SIL_INPUT for simulated debug connection (in)
      ["flight_computer:6", "SIL_OUTPUT"],
      ["SIL_INPUT", "telemetry_power_control:5"], // SIL_OUTPUT for simulated debug connection (out)
      ["telemetry_power_control:6", "SIL_OUTPUT"]
    ]
  }
]

```
#### Motor file format
```javascript
{
  "interpolation": "linear", // Type of interpolation between thrust curve points
  "thrust_curve": {          // Time from activation and thrust (N), must start and end with 0, must be in chronological order
    "0": 0,
    "0.5": 100,
    "5": 100,
    "5.5": 0
  }
}
```

## Adding output formats
* Create a .cc and .h file for the intended output (subclass of Output)
* Add relevant parsing code for the output in output_parser.cc and .h

## Weird things are happening (segfault, etc.) when running or compiling the program
* Try `make clean` first
