#ifndef MOTORS_HPP
#define MOTOR_HPP

#include "main.h"

using namespace std;
using namespace pros;
// if you add any motr motors remeber to use the extern keyword


extern andesine::aMotorGroup rightMotorGroup;
extern andesine::aMotorGroup leftMotorGroup;


/*
    @brief gradually changes the motor speed from the current speed to the target speed
    
    @param newSpeed the speed you want the robot to get to
    @param oldSpeed the current speed of the robot

    @note if the oldSpeed == newSpeed, the function returns oldSpeed
    @note if the oldSpeed > newSpeed, the function returns oldSpeed - 20
    @note if the oldSpeed < newSpeed, the function returns oldSpeed + 20

    @return int32_t 
*/
int32_t accelerate(int32_t, int32_t);
#endif