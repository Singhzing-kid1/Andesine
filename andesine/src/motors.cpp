/**
 * @file motors.cpp
 * @author Veer Singh
 * @brief defining motors and motorgroups
 * @version 0.1
 * @date 2024-10-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "main.h"

using namespace std;
using namespace pros;

Motor leftMotor1(1);
Motor leftMotor2(2);
Motor leftMotor3(3);

Motor rightMotor1(4);
Motor rightMotor2(6);
Motor rightMotor3(7);

Motor intake1(5);
Motor intake2(20);

andesine::aMotorGroup rightMotorGroup;
andesine::aMotorGroup leftMotorGroup;
andesine::aMotorGroup otherMotors;

void initializeMotors() {
    rightMotorGroup = make_tuple(vector<Motor>{rightMotor1, rightMotor2, rightMotor3}, MotorCartridge::blue, false, andesine::aMotorGroup::ID::RIGHT);
    leftMotorGroup = make_tuple(vector<Motor>{leftMotor1, leftMotor2, leftMotor3}, MotorCartridge::blue, true, andesine::aMotorGroup::ID::LEFT);
    otherMotors = make_tuple(vector<Motor>{intake1, intake2}, MotorCartridge::green, false, andesine::aMotorGroup::ID::OTHER);
}

int32_t accelerate(int32_t newSpeed, int32_t oldSpeed){
    if(newSpeed > oldSpeed){
        return oldSpeed += 20;
    } else if(newSpeed < oldSpeed){
        return oldSpeed -= 20;
    } else {
        return oldSpeed;
    }
}