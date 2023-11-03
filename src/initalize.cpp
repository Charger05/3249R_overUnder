#include "main.h"
#include "display/lv_objx/lv_label.h"
#include "initalize.hpp"

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	pros::lcd::register_btn1_cb(on_center_button);
	
	pros::ADIDigitalOut flapsPiston (1);

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
}