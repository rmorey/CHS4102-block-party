#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     SONAR,          sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     M_DRIVE_BL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     M_DRIVE_FL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     M_BELT,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_LIFT_L,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     M_DRIVE_BR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     M_DRIVE_FR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C3_1,     M_LIFT_R,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C3_2,     M_FLAG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    S_SCORE,              tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_2,    S_HOOK,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_3,    S_LID,                tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    S_AUTO,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


#include "JoystickDriver.c"
#include "robot.h"

#define PWR_LIFT 100
#define PWR_DRIVE 100
#define PWR_ROTATE 100
#define JS_THRESH 5 //slippage threshold
#define JS_SCALE (100.0/127) //scaling factor for joysticks

int drive_direction = 1; //1 for forwards, -1 for backwards
bool tilted_drive = false;
bool lid_closed = false;
int pwr_FR = 0;
int pwr_FL = 0;
int pwr_BR = 0;
int pwr_BL = 0;
int pwr_lift = 0;
int enc_lift_max = ENC_LIFT_MAX;

void InitializeRobot (){
	nMotorEncoder[M_LIFT_R] = 0;
	servo[S_LID] = 215;
	servo[S_AUTO] = 0;
	return;
}

task DriveUpdate() {
	//Drive control update loop
	while (true) {
		getJoystickSettings(joystick);

		int js_drive_x = JS_DRIVE_X*JS_SCALE;
		int js_drive_y = JS_DRIVE_Y*JS_SCALE;
		int js_rotate = JS_ROTATE*JS_SCALE;

		//slippage control
		if (abs(js_drive_x) < JS_THRESH) js_drive_x = 0;
		if (abs(js_drive_y) < JS_THRESH) js_drive_y = 0;
		if (abs(js_rotate) < JS_THRESH) js_rotate = 0;

		if (tilted_drive) { //titled drive mode, the flag spinner is onsidered the front of the robot
			pwr_FR = js_drive_y - js_rotate;
			pwr_FL = js_drive_x + js_rotate;
			pwr_BR = js_drive_x - js_rotate;
			pwr_BL = js_drive_y + js_rotate;
		}

		else {  //standard omniwheel drive

			pwr_FR = drive_direction*(js_drive_y - js_drive_x - drive_direction*js_rotate);
			pwr_FL = drive_direction*(js_drive_y + js_drive_x + drive_direction*js_rotate);
			pwr_BR = drive_direction*(js_drive_y + js_drive_x - drive_direction*js_rotate);
			pwr_BL = drive_direction*(js_drive_y - js_drive_x + drive_direction*js_rotate);
		}

		motor[M_DRIVE_FR] = pwr_FR;
		motor[M_DRIVE_FL] = pwr_FL;
		motor[M_DRIVE_BR] = pwr_BR;
		motor[M_DRIVE_BL] = pwr_BL;

		EndTimeSlice();
	}
}

task DriverButtons() {
	//update loop for driver buttons
	while(true) {
		// belt control
		while (BTN_BELT_IN) {
			motor[M_BELT] = 100;
			getJoystickSettings(joystick);
		}
		while (BTN_BELT_OUT) {
			motor[M_BELT] = -100;
			getJoystickSettings(joystick);
		}
		motor[M_BELT] = 0;

		//flag control
		while (BTN_FLAG) {
			motor[M_FLAG] = 100;
			getJoystickSettings(joystick);
		}
		motor[M_FLAG] = 0;

		//switches to the tilted drive mode
		if (BTN_TILT) tilted_drive = true;

		//backwards drive mode
		if (BTN_BACK) {
			tilted_drive = false;
			drive_direction = -1;
		}

		//revert to standard drive mode
		if (BTN_FRONT) {
			tilted_drive = false;
			drive_direction = 1;
		}

		EndTimeSlice();
	}
}

task ScorerButtons() {
	//update loop for scorer joystick button
	while(true) {

		//score blocks button
		if (BTN_SCORE) {
			servo[S_LID] = 250;
			lid_closed = false;
			scoreBlocks();
			wait1Msec(500);
		}

		//reset lift encoder to zero
		if (BTN_ZERO_LIFT) nMotorEncoder[M_LIFT_R] = 0;

		//sets max lift height to current height
		if (BTN_SETMAX_LIFT) enc_lift_max = nMotorEncoder[M_LIFT_R];

		//hook control
		while (BTN_HOOK_OUT) {
			servo[S_HOOK] = 64;
			getJoystickSettings(joystick);
		}
		while (BTN_HOOK_IN) {
			servo[S_HOOK] = 192;
			getJoystickSettings(joystick);
		}
		servo[S_HOOK] = 128;

		//block stopper control
		if (BTN_LID) {
			if (lid_closed) {
				servo[S_LID] = 250;
				lid_closed =false;
			}
			else {
				if (nMotorEncoder[M_LIFT_R} > 6000){
				servo[S_LID] = 0;
				lid_closed = true;
			}
		}
		wait1Msec(500);
	}

	if(joy2Btn(7)) {
		servo[S_AUTO] = 256;
	}

	EndTimeSlice();
}
}

task LiftUpdate() {
//lift control update loop
while(true) {

	//buttons to go straight to top/bottom
	if (BTN_RAISE_LIFT) {
		motor[M_LIFT_L] = 100;
		motor[M_LIFT_R] = 100;
		while (!BTN_STOP_LIFT && nMotorEncoder[M_LIFT_R] < enc_lift_max) {
			getJoystickSettings(joystick);
		}
		liftStop();
	}

	if (BTN_LOWER_LIFT) {
		motor[M_LIFT_L] = -100;
		motor[M_LIFT_R] = -100;
		while (!BTN_STOP_LIFT && nMotorEncoder[M_LIFT_R] > 0) {
			getJoystickSettings(joystick);
		}
		liftStop();
	}

	int js_lift = JS_LIFT;
	if (abs(js_lift) < JS_THRESH) js_lift = 0;

	int enc_R_lift = nMotorEncoder[M_LIFT_R];
	nxtDisplayString(0, "%8d", nMotorEncoder[M_LIFT_R]);

	if (!BTN_IGNORE_ENC && ((enc_R_lift < 0 && js_lift < 0) || (enc_R_lift > enc_lift_max && js_lift > 0))) {
		pwr_lift = 0;
	}
	else {
		pwr_lift = PWR_LIFT*(js_lift/JS_SCALE);
	}

	motor[M_LIFT_L] = pwr_lift;
	motor[M_LIFT_R] = pwr_lift;

	if (nMotorEncoder[M_LIFT_R] == 6000 && pwr_lift > 0) { //should be just above lexan thing, maybe make this fuzzy
		servo[S_LID] = 0; //close block stopper
		wait1Msec(50);
	}

	EndTimeSlice();
}
}


task main() {
InitializeRobot();
waitForStart();

StartTask(DriveUpdate);
StartTask(LiftUpdate);
StartTask(DriverButtons);
StartTask(ScorerButtons);

while(true) {
	getJoystickSettings(joystick);

	if(bDisconnected) { //kills motors and program if connection is lost
		haltAllMotors();
		StopAllTasks();
	}

	// Global motor kill
	if (BTN_KILL) {
		hogCPU();
		haltAllMotors();
		while(BTN_KILL) {
			getJoystickSettings(joystick);
		}
		releaseCPU();
	}

	EndTimeSlice();
}
}
