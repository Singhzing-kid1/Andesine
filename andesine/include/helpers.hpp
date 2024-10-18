#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "main.h"

using namespace std;
using namespace pros;

#pragma once

namespace andesine{
    class aMotorGroup{
        public:
            aMotorGroup();
            aMotorGroup(const vector<Motor>&, MotorGears);

            vector<Motor> motors;
            vector<int8_t> ports;
            MotorGears gearing;
            
            void operator=(const int32_t&);
            aMotorGroup& operator=(const pair<vector<Motor>, MotorGears>&);

            void brake();
    };
}
#endif