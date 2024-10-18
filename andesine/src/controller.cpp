#include "main.h"

using namespace std;
using namespace pros;

Controller master (E_CONTROLLER_MASTER);

bool isInside(int32_t x, int32_t y, int32_t radius){
    if(hypot(x, y) <= radius){
        return true;
    } else {
        return false;
    }
}

int32_t deadzone(controller_analog_e_t valueX, controller_analog_e_t valueY){
    bool insideCheck = isInside(master.get_analog(valueX), master.get_analog(valueY), 10);

    switch(insideCheck){
        case true:
            return 0;
            break;
        
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
}

bool closeEnough(int32_t leftSpeed, int32_t rightSpeed){
    return abs(rightSpeed - leftSpeed) <= 5 ? true : false;
}