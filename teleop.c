

#include "JoystickDriver.c"
#include "pragma.h"
#include "robot.h"
// dthis has all the button definitions

#define BUCKET_SPEED 50
#define DRIVE_SPEED 100
#define ROTATE_SPEED 100

bool tilted_drive = false;
float FRpower;
float FLpower;
float BRpower;
float BLpower;


void InitializeRobot (){
	bFloatDuringInactiveMotorPWM = false;
	return;
}

task DriveUpdate() {
	while (true) {
		getJoystickSettings(joystick);

		if (tilted_drive) {
			FLpower = (DRIVE_SPEED * DRIVE_JS_X) / 127.0;
			BRpower = (DRIVE_SPEED * DRIVE_JS_X) / 127.0;
			FRpower = (DRIVE_SPEED * DRIVE_JS_Y) / 127.0;
			BLpower = (DRIVE_SPEED * DRIVE_JS_Y) / 127.0;
		}

		else {
			FRpower = .7071*DRIVE_SPEED*(DRIVE_JS_Y + DRIVE_JS_X)/127.0 - ROTATE_SPEED*ROTATE_JS/127.0;
			FLpower = .7071*DRIVE_SPEED*(DRIVE_JS_Y + DRIVE_JS_X)/127.0 + ROTATE_SPEED*ROTATE_JS/127.0;
			BRpower = .7071*DRIVE_SPEED*(DRIVE_JS_Y - DRIVE_JS_X)/127.0 - ROTATE_SPEED*ROTATE_JS/127.0;
			BLpower = .7071*DRIVE_SPEED*(DRIVE_JS_Y - DRIVE_JS_X)/127.0 + ROTATE_SPEED*ROTATE_JS/127.0;
		}

		//slippage control
		if (abs(FRpower) < 10) {
			FRpower = 0;
		}
		if (abs(FLpower) < 10) {
			FLpower = 0;
		}
		if (abs(BRpower) < 10) {
			BRpower = 0;
		}
		if (abs(BLpower) < 10) {
			BLpower = 0;
		}

		motor[M_drive_FR] = FRpower;
		motor[M_drive_FL] = FLpower;
		motor[M_drive_BR] = BRpower;
		motor[M_drive_BL] = BLpower;

		int bucket_power = (BUCKET_SPEED*BUCKET_JS)/127.0;
		if (abs(BUCKET_JS) < 5) { //slippage control
			bucket_power = 0;
		}
		motor[M_slider_L] = bucket_power;
		motor[M_slider_R] = bucket_power;
		EndTimeSlice();
	}
}

task main() {
	InitializeRobot();


	waitForStart();
	StartTask(DriveUpdate);

	while(true) {
		getJoystickSettings(joystick);

		if(bDisconnected) { //kills motors and program if connection is losts
			haltAllMotors();
			StopAllTasks();
		}

		// belt control
		while (BELT_IN_BTN) {
			motor[M_belt] = 100;
			getJoystickSettings(joystick);
		}
		motor[M_belt] = 0;

		while (BELT_OUT_BTN) {
			motor[M_belt] = -100;
			getJoystickSettings(joystick);
		}
		motor[M_belt] = 0;

		if (SCORE_BTN) {
			scoreBlocks();
		}

		if(KILL_BTN) {  // Global abort
			haltAllMotors();
		}

		if (TILT_BTN) {
			tilted_drive = !tilted_drive;
		}

		EndTimeSlice();
	}
}
