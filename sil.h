#pragma once
#include "common.h"
#include "components/environment.h"
#include "components/rocket.h"
#include "includes/mbed.h"
#include <iostream>

using namespace std;

extern Environment* global_env;
extern int current_mcu;

shared_ptr<Rocket> curr_roc();