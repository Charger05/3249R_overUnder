#include "main.h"
#include "display/lv_objx/lv_label.h"
#include "initalize.hpp"

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

const double pi = 3.14159265359; // Define the number pi
const double circum4Omni = 4*pi;
const double innerCircum = 2*pi*6;//circumference of robot
const double ratio = 7/5;
const double botCircum = 2*pi*;//circumference of robot's turning circle


void clearChassisRotation(){
	rearLeft.tare_position();
	midLeft.tare_position();
	frontLeft.tare_position();
	ptoFront.tare_position();
	ptoRear.tare_position();
	rearRight.tare_position();
}
/*
moveChassis Function: This function takes a distance, in inches,
and converts it into a certain angle that the robot's motors will
rotate to.
Positive distance = the robot moves forward
Negative distance = the robot moves backward

Parameters:
(travel distance in inches, velocity of drive train's left side, velocity of drive train's right side)
*/
void moveChassis(double distance, int Vel) {
  clearChassisRotation(); // Call this function to prevent confusion from a
                          // previous function.
  double degGoal = (360 / circum4Omni) * distance; // Creates a variable that stores the degree to
                             // determine how many times the motors will rotate.
  double degC = 0; // Creates a variable to store the drivetrain's motor
                   // positions in degrees.
  if (degGoal > 0) {
    // Move the robot forwards while the robot hasn't reached the specified
    // distance.
    while (degC < degGoal) {
      /*
      1. Calculate the drivetrain's motor positions in degrees by getting the
      average value of all the drivetrain's motors.
      2. Store the average value of the drivetrain's motor positions.
      */
      degC = (rearLeft.get_position() + (midLeft.get_position()/(21/5))+ frontLeft.get_position() + rearRight.get_position() + ptoRear.get_position() + ptoFront.get_position()) /6;
      // Each drivetrain motor's values are negated because the motors
      // are reversed.
      rearLeft.move_Velocity(Vel);
      midLeft.move_Velocity(Vel/(21/5));
	  frontLeft.move_Velocity(Vel);
	  rearRight.move_Velocity(Vel);
	  ptoFront.move_Velocity(Vel);
	  ptoRear.move_Velocity(Vel);
    }
  } else if (degGoal < 0) {
    // Move the robot backwards while the robot hasn't reached the specified
    // distance.
    while (degC > degGoal) {
      // Same calculations that are in the previous while loop.
      degC = (leftFront.rotation(degrees) + leftRear.rotation(degrees) + (rightFront.rotation(degrees) * -1) + (rightRear.rotation(degrees) * -1)) / 4;
      // Each drivetrain motor's values are negated because the motors
      // are reversed.
      LeftDriveSmart.spin(reverse);
      RightDriveSmart.spin(forward);
    }
  }
  LeftDriveSmart.stop(brake);
  RightDriveSmart.stop(brake);
}
/*
This function turns the bot to the right by pivoting it on its right side.
Positive angle = the robot pivots forward
Negative angle = the robot pivots backward
*/
void pivotRight(double degr) {
  clearChassisRotation(); // Call this function to prevent confusion from a
                          // previous function.
  double distG = (botCircum/ (360*ratio)) * degr; // Creates a variable that stores the distance, in inches, of how
            // much the robot's left side will have to cover
  double distC = 0; // Creates a variable to store the distance covered by the
                    // left side of the drivetrain in inches.
  if (distG > 0) {
    // Pivot the robot forwards while the robot hasn't met the desired angle.
    while (distC < distG) {
      /*
      Calculate the distance covered by the left side drivetrain's motors in
      inches by:
      1. Retrieving the left front motor's angle, in degrees.
      2. Converting them into inches.
      Then:
      Store the average value of the distance covered by the drivetrain's left
      side motors.
      */
      distC = leftFront.rotation(degrees) * ((circum4Omni) / 360*(ratio)) * 0.845;
      LeftDriveSmart.spin(forward);
      RightDriveSmart.stop(brake);
    }
  } else if (distG < 0) {
    // Pivot the robot backwards while the robot hasn't met the desired angle.
    while (distC > distG) {
      // Same calculations that are in the above while loop.
      distC = leftFront.rotation(degrees) * ((circum4Omni) / 360*(ratio)) * 0.845;
      LeftDriveSmart.spin(reverse);
      RightDriveSmart.stop(brake);
    }
  }
  LeftDriveSmart.stop(brake);
  RightDriveSmart.stop(brake);
}

void rotateRight(double degr) {
  LeftDriveSmart.setVelocity(50, percent);
  RightDriveSmart.setVelocity(50, percent);
  clearChassisRotation(); // Call this function to prevent confusion from a
                          // previous function.
  double distG = (innerCircum/ (360*ratio)) * degr; // Creates a variable that stores the distance, in inches, of how
            // much the robot's left side will have to cover
  double distC = 0; // Creates a variable to store the distance covered by the
                    // left side of the drivetrain in inches.
  if (distG > 0) {
    // Pivot the robot forwards while the robot hasn't met the desired angle.
    while (distC < distG) {
      /*
      Calculate the distance covered by the left side drivetrain's motors in
      inches by:
      1. Retrieving the left front motor's angle, in degrees.
      2. Converting them into inches.
      Then:
      Store the average value of the distance covered by the drivetrain's left
      side motors.
      */
      distC = ((leftFront.rotation(degrees) + rightFront.rotation(degrees))/2) * ((circum4Omni) / 360*(ratio)) * 0.845;
      LeftDriveSmart.move_Velocity(forward);
      RightDriveSmart.move_Velocity(forward);//forward = reverse
    }
  } else if (distG < 0) {
    // Pivot the robot backwards while the robot hasn't met the desired angle.
    while (distC > distG) {
      // Same calculations that are in the above while loop.
      distC = ((leftFront.rotation(degrees) + rightFront.rotation(degrees))/2) * ((circum4Omni) / 360*(ratio)) * 0.845;
      LeftDriveSmart.spin(reverse);
      RightDriveSmart.spin(reverse);//reverse = forward
    }
  }
  LeftDriveSmart.stop(brake);
  RightDriveSmart.stop(brake);
}


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
void autonomous() {

	//Driver Skills

	rearLeft.move_velocity(200);
	midLeft.move_velocity(600);
	frontLeft.move_velocity(200);
	ptoFront.move_velocity(200);
	ptoRear.move_velocity(200);
	rearRight.move_velocity(200);
	pros::delay(2000);
	rearLeft.move_velocity(-200);
	midLeft.move_velocity(-600);
	frontLeft.move_velocity(-200);
	ptoFront.move_velocity(-200);
	ptoRear.move_velocity(-200);
	rearRight.move_velocity(-200);
	pros::delay(500);
	rearLeft.move_velocity(0);
	midLeft.move_velocity(0);
	frontLeft.move_velocity(0);
	ptoFront.move_velocity(0);
	ptoRear.move_velocity(0);
	rearRight.move_velocity(0);
	
	
	//Auton Skills
	/*
	rearLeft.move_velocity(200);
	midLeft.move_velocity(600);
	frontLeft.move_velocity(200);
	ptoFront.move_velocity(200);
	ptoRear.move_velocity(200);
	rearRight.move_velocity(200);
	pros::delay(150);
	rearLeft.move_velocity(0);
	midLeft.move_velocity(0);
	frontLeft.move_velocity(0);
	ptoFront.move_velocity(0);
	ptoRear.move_velocity(0);
	rearRight.move_velocity(0);

	cataMotor.move_velocity(100);
	pros::delay(30000);
	cataMotor.move_velocity(0);
	pros::delay(500);
	rearLeft.move_velocity(-100);
	midLeft.move_velocity(-300);
	frontLeft.move_velocity(-100);
	ptoFront.move_velocity(-100);
	ptoRear.move_velocity(-100);
	rearRight.move_velocity(-100);
	pros::delay(5000);
	rearLeft.move_velocity(0);
	midLeft.move_velocity(0);
	frontLeft.move_velocity(0);
	ptoFront.move_velocity(0);
	ptoRear.move_velocity(0);
	rearRight.move_velocity(0);
	*/
	
	
}

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

	//for pto
	bool pto = true; //true = chassis, false = lift
	
	//for wings
	bool wing = false;//true = expanded, false = retracted

	//for catapult
	bool cataAdj = false;//false = auto launch, true = manually adjust gear
	
	//for chassis controls
	int power = 0;
	int turn = 0;
	int left = 0;
	int right = 0;

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
		/*
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
			if(pto){
				pto = false;
			}
			else{
				pto = true;
			}
			pros::ADIDigitalOut ptoPiston (#, pto);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
				pros::delay(2);
			}
		}
		*/
		
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
			if(wing){
				wing = false;
			}
			else{
				wing = true;
			}
			pros::ADIDigitalOut flapsPiston (1, wing);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
				pros::delay(2);
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
				cataMotor.move_absolute(900, 200);
				int delay = 0;
				while(!((cataMotor.get_position() < 905) && (cataMotor.get_position() > 895))){
					pros::delay(2);
					delay++;
					if(delay > 500){
						break;
					}
				}
				/*
				while(!cataButton.get_value()){
					cataMotor.move_velocity(120);
				}
				*/
				
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
