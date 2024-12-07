/**
 * @file controller.cpp
 * @author Veer Singh
 * @brief controller input handling functions
 * @version 0.1
 * @date 2024-10-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "main.h"

using namespace std;
using namespace pros;

Controller master(E_CONTROLLER_MASTER);

bool isInside(int32_t x, int32_t y, int32_t radius){
    return (hypot(x, y) <= radius);
}

int32_t deadzone(controller_analog_e_t valueX, controller_analog_e_t valueY, int32_t deadzone){
    bool insideCheck = isInside(master.get_analog(valueX), master.get_analog(valueY), deadzone);

    switch(insideCheck){
        case true:
            return 0;
        
        case false:
            if(master.get_analog(valueX) != 0){
                if(master.get_analog(valueY) > 0){
                    return (int32_t)hypot(master.get_analog(valueX), master.get_analog(valueY));
                } else if(master.get_analog(valueY) < 0) {
                    return -(int32_t)hypot(master.get_analog(valueX), master.get_analog(valueY));
                }
            } else {
                return master.get_analog(valueY);
            }
    }

    return 0;
}

bool closeEnough(int32_t leftSpeed, int32_t rightSpeed){
    return (abs(rightSpeed - leftSpeed) <= 5);
}