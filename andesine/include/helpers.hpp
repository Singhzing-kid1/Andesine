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
            aMotorGroup(const vector<Motor>&, MotorGears, bool);

            vector<Motor> motors;
            vector<int8_t> ports;
            MotorGears gearing;
            bool reversed;

            void operator=(const int32_t&);
            aMotorGroup& operator=(const tuple<vector<Motor>, MotorGears, bool>&);

            void brake();
    };
}


#endif