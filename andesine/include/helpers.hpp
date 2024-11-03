#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "main.h"

using namespace std;
using namespace pros;

#pragma once

namespace andesine{
    class aMotorGroup{
        public:
            enum ID {LEFT, RIGHT, OTHER};

            aMotorGroup();
            aMotorGroup(const vector<Motor>&, MotorGears, ID, bool);

            vector<Motor> motors;
            vector<int8_t> ports;
            MotorGears gearing;
            bool reversed;
            ID id;

            void operator=(const int32_t&);
            aMotorGroup& operator=(const tuple<vector<Motor>, MotorGears, bool, ID>&);

            void brake();
    };

    class logger{
        public:
            enum controlMode {PIT = 0, SKILLS_OPCONTROL = 1, SKILLS_AUTON = 2, OPCONTROL = 3, AUTON = 4};

            logger(controlMode);
            ~logger();

            // Drive train Motor Groups, batter temp, battery voltage, capacity, current draw, controller, sd card installed. 
            void writeToBuffer(vector<aMotorGroup>&, Controller, uint32_t);
            void writeToFile();

        private:
            ofstream logFile;
            fstream nameRules;

            bool sdInstalled;

            controlMode mode;

            ostringstream buffer;

            string nameGen(fstream*);
            void createFile(string);
    };
}


#endif