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
	pros::ADIDigitalOut ptoPiston (1);

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
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor rearLeft(1);
	pros::Motor midLeft(5, MOTOR_GEARSET_06);
	midLeft.set_reversed(true);
	pros::Motor frontLeft(4);
	pros::Motor ptoFront(2, true);
	pros::Motor ptoRear(3);
	pros::Motor rearRight(6, true);
	pros::Motor intakeMotor(7, true);
	pros::Motor cataMotor(8, true);

	pros::ADIDigitalIn cataButton (2);

	bool pto = true; //true = chassis, false = lift
	bool cataAdj = false;//false = auto launch, true = manually adjust gear
	
	//for chassis controls
	int power = 0;
	int turn = 0;
	int left = 0;
	int right = 0;

	//for catapult
	double goPos = 0;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		
		power = master.get_analog(ANALOG_LEFT_Y);
    	turn = master.get_analog(ANALOG_RIGHT_X);
		left = power + turn;
		right = power - turn;

		rearLeft.move(left);
		midLeft.move(left/(21/5));
		frontLeft.move(left);
		
		rearRight.move(right);
		
		if(pto){
			ptoFront.move(right);
			ptoRear.move(right);
		}
		else{
			ptoFront = 0;
			ptoRear = 0;
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
			if(pto){
				pto = false;
			}
			else{
				pto = true;
			}
		}
		
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			intakeMotor.move_velocity(200);
		}
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			intakeMotor.move_velocity(-200);
		}
		else{
			intakeMotor.move_velocity(0);
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
			if(!cataAdj){
				cataAdj = true;
			}
			else{
				cataAdj = false;
				cataMotor.tare_position();
			}
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
				pros::delay(2);
			}
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			if(!cataAdj){
				while(!cataButton.get_value()){
					cataMotor.move_velocity(120);
				}
				cataMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
				cataMotor.tare_position();
			}
			else if(cataAdj){
				while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
					cataMotor.move_velocity(50);
				}
				cataMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			}
			
		}
		else{
			cataMotor.move_velocity(0);
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			if(cataAdj){
				while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
					cataMotor.move_velocity(-50);
				}
				cataMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
			}
		}
		else{
			if(!master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
				cataMotor.move_velocity(0);
			}
			
		}

		pros::delay(20);
	}
}
