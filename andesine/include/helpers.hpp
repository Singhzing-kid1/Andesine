#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "main.h"

using namespace std;
using namespace pros;

#pragma once

namespace andesine{
    class aMotorGroup{
        public:
            enum ID {LEFT, RIGHT, OTHER}

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

    class logger{
        public:
            enum controlMode {PIT, SKILLS_OPCONTROL, SKILLS_AUTON, OPCONTROL, AUTON};

            logger(controlMode);
            ~logger();

            // Drive train Motor Groups, batter temp, battery voltage, capacity, current draw, controller, sd card installed. 
            void writeToBuffer(vector<aMotorGroup>, Controller);
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