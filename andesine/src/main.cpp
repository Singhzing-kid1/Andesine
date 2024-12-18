#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

bool lastState = false;
bool risingEdgeDetected = false;

bool risingEdgeDetector(bool currentState){
	risingEdgeDetected = currentState && !lastState;
	lastState = currentState;
	return risingEdgeDetected;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
	initializeMotors();

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	int32_t avgSpeed = 0;
	int32_t leftSpeed = 0;
	int32_t rightSpeed = 0;

	uint32_t deltaTime = 0;
	uint32_t currentFrame = 0;
	uint32_t lastFrame = 0;

	andesine::logger opControlLogger(andesine::logger::controlMode::OPCONTROL);
	andesine::user user(andesine::user::userID::LEO, 10, master);

	adi::Pneumatics air('a', false);

	andesine::risingEdgeDetector edgerL1;
	andesine::risingEdgeDetector edgerL2;
	andesine::risingEdgeDetector edgerA;

	andesine::user::accelCurve activeCurve = user.userDefault;

	vector<andesine::aMotorGroup> motors = {leftMotorGroup, rightMotorGroup, otherMotors};

	while (true) {
		currentFrame = millis();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		leftSpeed = user.accelerate(deadzone(ANALOG_LEFT_X, ANALOG_LEFT_Y, user.deadzone), activeCurve);
		rightSpeed = user.accelerate(deadzone(ANALOG_RIGHT_X, ANALOG_RIGHT_Y, user.deadzone), activeCurve);

		edgerL1.setInput(master.get_digital(DIGITAL_L1));
		edgerL2.setInput(master.get_digital(DIGITAL_L2));
		edgerA.setInput(master.get_digital(DIGITAL_A));

		if(master.get_analog(ANALOG_RIGHT_Y) != 0 || master.get_analog(ANALOG_LEFT_Y) != 0){
			switch(closeEnough(leftSpeed, rightSpeed)){
				case true:
					avgSpeed = 0.5*(leftSpeed + rightSpeed);
					leftMotorGroup = avgSpeed;
					rightMotorGroup = avgSpeed;
					break;

				case false:
					leftMotorGroup = leftSpeed;
					rightMotorGroup = rightSpeed;
					break;
			}
		} else {
			leftMotorGroup.brake();
			rightMotorGroup.brake();
		}


		if(edgerA.checkRisingEdge()){
			switch(activeCurve){
				case andesine::user::accelCurve::LOG:
					activeCurve = andesine::user::accelCurve::SIGMOID;
					master.print(1, 0, "Sigmoid");
					break;

				case andesine::user::accelCurve::SIGMOID:
					activeCurve = andesine::user::accelCurve::LOG;
					master.print(1, 0, "Log    ");
					break;
			}
		}

		if(edgerL1.checkRisingEdge()){
			air.extend();
		}

		if(edgerL2.checkRisingEdge()){
			air.retract();
		}

		if(master.get_digital(DIGITAL_R1)){
			intake1.move(-127);
		} else {
			intake1.brake();
		}

		if(master.get_digital(DIGITAL_R2)){
			intake2.move(-127);
		} else {
			intake2.brake();

		}



		opControlLogger.writeToBuffer(motors, master, currentFrame);

		if(currentFrame % 20 == 0){
			opControlLogger.writeToFile();
		}

		if(currentFrame >= 104980){
			opControlLogger.~logger();
		}
	}
}