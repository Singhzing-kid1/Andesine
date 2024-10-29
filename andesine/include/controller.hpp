#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "main.h"

using namespace std;
using namespace pros;

extern Controller master;


/*
    @brief Checks if a point made from the x, y values of the analog sticks is in a precribed circle

    @oaram x x value of the analog stick
    @param y y value of the analog stick
    @param radius radius of the circle that the points are being checked against

    @returns bool
*/
bool isInside(int32_t, int32_t, int32_t);


/*
    @brief runs isInside

    @param valueX the x analog stick axis
    @param valueY the y analog stick axis

    @note if the insideCheck returns true the function will return 0
    @note if the insideCheck return false the function will return either the hypot of the valueX,valueY or just valueY

    @returns int32_t
*/
int32_t deadzone(controller_analog_e_t, controller_analog_e_t);

/*
    @brief checks if the two analog stick inputs are close together

    @param leftSpeed the value of the left analog stick
    @param rightSpeed the valye of the right analog stick

    @note will return false if the inputs are not within +/-5 of each other
    @note will retun true if the are within +/-5 of each other

    @returns bool
*/
bool closeEnough(int32_t, int32_t);
#endif