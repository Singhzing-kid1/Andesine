#ifndef MOTORS_HPP
#define MOTORS_HPP

#include "main.h"

using namespace std;
using namespace pros;

extern andesine::aMotorGroup rightMotorGroup;
extern andesine::aMotorGroup leftMotorGroup;

void initializeMotors();
int32_t accelerate(int32_t, int32_t);

#endif