#include "main.h"

int clamp(int input, int minint = -127, int maxint = 127) {
	if (maxint < input) { return maxint; } else
	if (input < minint) { return minint; } else
	return input;
};

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor drv_l(12,true); // Left drive
	pros::Motor drv_r(19); // Right drive
	pros::Motor arm_l(11,true); // Lower spinner wheel */
	pros::Motor arm_r(20); // Upper spinner wheel
	pros::Motor manum(16); // Claw/hand/grabby thingy


	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = clamp(master.get_analog(ANALOG_LEFT_Y)); // Left drive global values
		int right = clamp(master.get_analog(ANALOG_RIGHT_Y)); // Right drive global values
		int arm_speed = 127;

		drv_l.move(left);
		drv_r.move(right);

		if (master.get_digital(DIGITAL_R1)) { arm_l.move(arm_speed); arm_r.move(arm_speed); } else
		if (master.get_digital(DIGITAL_L1)) { arm_l.move(-1*arm_speed); arm_r.move(-1*arm_speed); } else
		{arm_l.move(0);arm_r.move(0);};
		
		if (master.get_digital(DIGITAL_R2)) { manum.move(arm_speed); } else
		if (master.get_digital(DIGITAL_L2)) { manum.move(-1*arm_speed); } else
		{manum.move(0);};

		pros::delay(20);
	}
}
