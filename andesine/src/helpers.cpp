/**
 * @file helpers.cpp
 * @author Veer
 * @brief any custom helper go here
 * @version 0.1
 * @date 2024-10-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "main.h"

using namespace std;
using namespace pros;
using namespace andesine;

namespace andesine{
    aMotorGroup::aMotorGroup(){}

    aMotorGroup::aMotorGroup(const vector<Motor>& motors, MotorGears gearing){
        vector<int8_t> ports;
        this->gearing = gearing;

        for(auto motor : motors){
            ports.push_back(motor.get_port());
            this->motors.push_back(motor);
        }

        this->ports = ports;
    }

    void aMotorGroup::operator=(const int32_t& speed){
        for_each(
            execution::par,
            motors.begin(),
            motors.end(),
            [speed](Motor& motor){
                motor.move(speed);
            }
        );
    }

    aMotorGroup& aMotorGroup::operator=(const pair<vector<Motor>, MotorGears>& params){
        for(auto motor : params.first){
            motors.push_back(motor);
        }
        gearing = params.second;

        return *this;
    }

    void aMotorGroup::brake(){
        for_each(
            execution::par,
            motors.begin(),
            motors.end(),
            [](Motor& motor){
                motor.brake();
            }
        );
    }
}