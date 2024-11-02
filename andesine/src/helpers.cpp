/**
 * @file helpers.cpp
 * @author Veer Singh
 * @brief helpers for coding the robot
 * @version 0.1
 * @date 2024-10-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

 #include "main.h"

 using namespace std;
 using namespace pros;

 namespace andesine{
    aMotorGroup::aMotorGroup(){}

    aMotorGroup::aMotorGroup(const vector<Motor>& motors, MotorGears gearing, bool reversed = false){
        for(auto motor : motors){
            ports.push_back(motor.get_port());
            this->motors.push_back(motor);
        }

        this->gearing = gearing;
        this->reversed = reversed;
    }

    void aMotorGroup::operator=(const int32_t& speed){
        for_each(
            motors.begin(),
            motors.end(),
            [speed, this](Motor& motor){
                this->reversed ? motor.move(-speed) : motor.move(speed);
            }
        );
    }

    aMotorGroup& aMotorGroup::operator=(const tuple<vector<Motor>, MotorGears, bool>& params){
        for(auto motor : get<0>(params)){
            motors.push_back(motor);
            ports.push_back(motor.get_port());
        }

        gearing = get<1>(params);
        reversed = get<2>(params);

        return *this;
    }

    void aMotorGroup::brake(){
        for_each(
            motors.begin(),
            motors.end(),
            [](Motor& motor){
                motor.brake();
            }
        );
    }
    

    logger::logger(logger::controlMode controlMode){
        mode = controlMode;
        nameRules.open("/usd/nameRules.txt");

        if(nameRules.is_open()){
            sdInstalled = true;
        } else {
            sdInstalled = false;
        }

        nameRules.close();

        createFile(nameGen(&nameRules));
    }

    logger::~logger(){
        writeToFile();
        logFile.close();
    }

    void writeToBuffer(vector<aMotorGroup> drivetrainMotors, Controller controller){
        ostringstream leftMotors;
        ostringstream rightMotors;
        ostringstream otherMotors;

        for (auto motorGroup : drivetrainMotors){

        }
    }
 }