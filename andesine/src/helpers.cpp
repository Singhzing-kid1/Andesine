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

    aMotorGroup::aMotorGroup(const vector<Motor>& motors, MotorGears gearing, ID id, bool reversed = false){
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

    aMotorGroup& aMotorGroup::operator=(const tuple<vector<Motor>, MotorGears, bool, ID>& params){
        for(auto motor : get<0>(params)){
            motors.push_back(motor);
            ports.push_back(motor.get_port());
        }

        gearing = get<1>(params);
        reversed = get<2>(params);
        id = get<3>(params);

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

        createFile(nameGen(&nameRules));
    }

    logger::~logger(){
        writeToFile();
        logFile.close();
    }

    void logger::writeToBuffer(vector<aMotorGroup>& drivetrainMotors, Controller controller, uint32_t time){
        ostringstream leftMotorTemps;
        ostringstream rightMotorTemps;
        ostringstream otherMotorTemps;

        ostringstream leftMotorEncoders;
        ostringstream rightMotorEncoders;
        ostringstream otherMotorEncoders;

        ostringstream leftMotors;
        ostringstream rightMotors;
        ostringstream otherMotors;

        ostringstream controllerStuff;

        ostringstream batteryStuff;

        for (auto motorGroup : drivetrainMotors){
            switch(motorGroup.id){
                case aMotorGroup::ID::LEFT:
                    for (auto motor : motorGroup.motors){
                        leftMotorTemps << motor.get_temperature() << ", ";
                        leftMotorEncoders << motor.get_position() << ", ";
                    }

                    leftMotors << leftMotorTemps.str() << ", " << leftMotorEncoders.str();
                    break;

                case aMotorGroup::ID::RIGHT:
                    for (auto motor : motorGroup.motors){
                        rightMotorTemps << motor.get_temperature() << ", ";
                        rightMotorEncoders << motor.get_position() << ", ";
                    }

                    rightMotors << rightMotorTemps.str() << ", " << rightMotorEncoders.str();
                    break;

                case aMotorGroup::ID::OTHER:
                    for (auto motor : motorGroup.motors){
                        otherMotorTemps << motor.get_temperature() << ", ";
                        otherMotorEncoders << motor.get_position() << ", ";
                    }

                    otherMotors << otherMotorTemps.str() << ", " << otherMotorEncoders.str();
                    break;
            }
        }

        controllerStuff << controller.get_analog(ANALOG_LEFT_Y) << ", " << controller.get_analog(ANALOG_LEFT_X) << ", " << controller.get_analog(ANALOG_RIGHT_Y) << ", " << controller.get_analog(ANALOG_RIGHT_X) << ", " << controller.get_digital(DIGITAL_L1) << ", " << controller.get_digital(DIGITAL_L2) << ", " << controller.get_digital(DIGITAL_R1) << ", " << controller.get_digital(DIGITAL_R2) << ", " << controller.get_digital(DIGITAL_A) << ", " << controller.get_digital(DIGITAL_Y) << ", ";
        batteryStuff << battery::get_temperature() << ", " << battery::get_current() << ", " << battery::get_capacity() << ", " << controller.get_battery_level();

        buffer << time << ", " << leftMotors.str() << ", " << rightMotors.str() << ", " << otherMotors.str() << ", " << controllerStuff.str() << batteryStuff.str() << "\n";
    }

    void logger::writeToFile(){
        logFile << buffer.str();
        logFile.flush();
        buffer.str("");
        buffer.clear();
    }

    string logger::nameGen(fstream* nameRulesFile){
        string competition, match, name;

        getline(*nameRulesFile, competition);
        getline(*nameRulesFile, match);

        name += competition;
        name += match;

        name += "." + to_string(mode);
        name += ".log";

        cout << name;

        return name;
    }

    void logger::createFile(string name){
        logFile.open("/usd/" + name, ios_base::app);
    }

    user::user(userID user, int32_t deadzone, Controller controller){
        id = user;
        userDefault = defaultAccelCurves[user];
        this->deadzone = deadzone;

        switch (user){
            case userID::LEO:
                controller.print(0, 0, "Welcome Leo");
                switch (userDefault){
                    case accelCurve::LOG:
                        controller.print(1, 0, "Log");
                        break;

                    case accelCurve::SIGMOID:
                        controller.print(1, 0, "Sigmoid");
                        break;
                }
                break;

            case userID::OTHER:
                controller.print(0, 0, "Welcome");
                switch (userDefault){
                    case accelCurve::LOG:
                        controller.print(1, 0, "You are using the LOG curve");
                        break;

                    case accelCurve::SIGMOID:
                        controller.print(1, 0, "You are using the Sigmoid curve");
                        break;
                }
                break;
            
        }
    }

    int32_t user::accelerate(int32_t speed, accelCurve curve){
        int32_t output;
        switch (id){
            case userID::LEO:
                switch (curve){
                    case accelCurve::LOG:
                        output = functionCalls[accelCurve::LOG](10, -0.075, speed); 
                        break;

                    case accelCurve::SIGMOID:
                        output = functionCalls[accelCurve::SIGMOID](8.7, 0.3, speed);
                }

        }

        return output;
    }

    int32_t user::sigmoid(double steepness, double midpoint, int32_t input){
        int sign = input/abs(input);
        cout << sign * 127 * (1/(1+exp((-steepness*((abs(input)/127) - midpoint))))) << "\n";
        
        return sign * 127 * (1/(1+exp((-steepness*((abs(input)/127) - midpoint)))));
    }

    int32_t user::exponential(double position, double expon, int32_t input){
        double sign = input/abs(input);
        
        double output = 0;
        double topExpon = (expon * abs((double)input - position));
        double botExpon = 127 * expon;

        double top = 16129 * (exp(topExpon) - (double)1);
        double bottom = 127 * (exp(botExpon) - (double)1);

        output = floor(top / bottom);

        return input == 0 ? 0 : sign * (int32_t)output;
        
    }
 }