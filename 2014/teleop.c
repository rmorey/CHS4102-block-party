#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     SONAR,          sensorSONAR)
#pragma config(Sensor, S4,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     M_DRIVE_BL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     M_DRIVE_FL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     M_BELT,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_LIFT_L,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S2_C1_1,     M_DRIVE_BR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     M_DRIVE_FR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C3_1,     M_LIFT_R,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C3_2,     M_FLAG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C2_1,    SV_BUCKET_L,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C2_2,    SV_HOOK,              tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C2_3,    SV_BUCKET_R,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C2_4,    SV_AUTO,              tServoStandard)
#pragma config(Servo,  srvo_S2_C2_5,    SV_LID,               tServoStandard)
#pragma config(Servo,  srvo_S2_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "robot.h"

#define THRESH 5 //slippage threshold
#define SCALE (100.0/127) //scaling factor for joysticks

int drive_direction = 1;
bool tilted_drive = false;
bool lid_closed = false;
int bucket_status = 0;
int pwr_FR = 0;
int pwr_FL = 0;
int pwr_BR = 0;
int pwr_BL = 0;
int pwr_lift = 0;
int enc_lift_max = ENC_LIFT_MAX;
const int SV_LID_CLOSED = 80;
const int SV_LID_OPEN = 245;

void InitializeRobot() {
	//disableDiagnosticsDisplay();
	nMotorEncoder[M_LIFT_L] = 0;
	servo[SV_AUTO] = 0;
	servo[SV_HOOK] = 245;
	servo[SV_LID] = SV_LID_OPEN;
}


task DriveUpdate() {
	//Drive control update loop
	while (true) {
		getJoystickSettings(joystick);

		int js_drive_x = JS_DRIVE_X*SCALE;
		int js_drive_y = JS_DRIVE_Y*SCALE;
		int js_rotate = JS_ROTATE*SCALE;
		int js_conveyor = JS_CONVEYOR*SCALE;

		//slippage control
		if (abs(js_drive_x) < THRESH)
			js_drive_x = 0;
		if (abs(js_drive_y) < THRESH)
			js_drive_y = 0;
		if (abs(js_rotate) < THRESH)
			js_rotate = 0;
		if (abs(js_conveyor) < THRESH)
			js_conveyor = 0;

		servo[SV_BUCKET_L] = js_conveyor/2 + 128;
		servo[SV_BUCKET_R] = -js_conveyor/2 + 128;

		if (tilted_drive) { //titled drive mode, the flag spinner is onsidered the front of the robot
			pwr_FR = js_drive_y - js_rotate;
			pwr_FL = js_drive_x + js_rotate;
			pwr_BR = js_drive_x - js_rotate;
			pwr_BL = js_drive_y + js_rotate;
		}
		else {  //standard omniwheel drive
			pwr_FR = drive_direction*(-js_drive_x + js_drive_y - drive_direction*js_rotate);
			pwr_FL = drive_direction*(js_drive_x + js_drive_y + drive_direction*js_rotate);
			pwr_BR = drive_direction*(js_drive_x + js_drive_y - drive_direction*js_rotate);
			pwr_BL = drive_direction*(-js_drive_x + js_drive_y + drive_direction*js_rotate);
		}

		motor[M_DRIVE_FR] = pwr_FR;
		motor[M_DRIVE_FL] = pwr_FL;
		motor[M_DRIVE_BR] = pwr_BR;
		motor[M_DRIVE_BL] = pwr_BL;

		motor[M_FLAG] = BTN_FLAG*100;

		EndTimeSlice();
	}
}

task DriverButtons() {
	//update loop for driver buttons
	while(true) {
		// belt control
		getJoystickSettings(joystick);
		motor[M_BELT] = 100*(BTN_BELT_IN - BTN_BELT_OUT);


		if (BTN_BUCKET_OUT){
			if (bucket_status == 1)
				bucket_status = 0;
			else
				bucket_status = 1;
			wait1Msec(100);
		}

		if (BTN_BUCKET_IN){
			if (bucket_status == -1)
				bucket_status = 0;
			else
				bucket_status = -1;
			wait1Msec(100);
		}

		if (BTN_TILT)
			tilted_drive = true;

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
	}
	EndTimeSlice();
}

task ScorerButtons() {
	//update loop for scorer joystick button
	while(true) {

		if (BTN_SCORE){
			servo[SV_LID] = SV_LID_OPEN;
			lid_closed = false;
		}

		if (BTN_HOOK_OUT)
			servo[SV_HOOK] = 245;
		if(BTN_HOOK_IN)
			servo[SV_HOOK] = 25;



		//reset lift encoder to zero
		if (BTN_ZERO_LIFT)
			nMotorEncoder[M_LIFT_L] = 0;

		//sets max lift height to current height
		if (BTN_SETMAX_LIFT)
			enc_lift_max = nMotorEncoder[M_LIFT_L];

		//hook control

		//block stopper control
		if (BTN_LID) {
			if (lid_closed) {
				servo[SV_LID] = SV_LID_OPEN;
				lid_closed =false;
			}
			else {
				if (nMotorEncoder[M_LIFT_L] > 6000){
					servo[SV_LID] = SV_LID_CLOSED;
					lid_closed = true;
				}
			}
			wait1Msec(500);
		}

		if (abs(nMotorEncoder[M_LIFT_L] - 6000) < 250 && pwr_lift > 0){
			servo[SV_LID] = SV_LID_CLOSED;
			lid_closed = true;
		}

		if (servo[SV_BUCKET_L] > 128){
			servo[SV_LID] = SV_LID_OPEN;
			lid_closed = false;
		}

		EndTimeSlice();
	}
}

task LiftUpdate() {
	//lift control update loop
	while(true) {

		//buttons to go straight to top/bottom
		if (BTN_RAISE_LIFT) {
			pwr_lift = 100;
			motor[M_LIFT_L] = 100;
			motor[M_LIFT_R] = 100;
			ClearTimer(T1);
			int stop_time = 6000; //ms
			while (!BTN_STOP_LIFT && nMotorEncoder[M_LIFT_L] < enc_lift_max && time1[T1] < stop_time) {
				getJoystickSettings(joystick);
			}
			liftStop();
		}

		if (BTN_LOWER_LIFT) {
			motor[M_LIFT_L] = -100;
			motor[M_LIFT_R] = -100;
			ClearTimer(T1);
			int stop_time = 6000; //ms
			while (!BTN_STOP_LIFT && nMotorEncoder[M_LIFT_L] > 0 && time1[T1] < stop_time) {
				getJoystickSettings(joystick);
			}
			liftStop();
		}

		//gets and scales joystick value
		int js_lift = JS_LIFT*SCALE;

		if (abs(js_lift) < THRESH)
			js_lift = 0;

		int enc_R_lift = nMotorEncoder[M_LIFT_L];
		nxtDisplayString(0, "%8d", nMotorEncoder[M_LIFT_L]);
		nxtDisplayString(2, "%8d", nMotorEncoder[M_DRIVE_FR]);
		nxtDisplayString(3, "%8d", nMotorEncoder[M_DRIVE_FL]);
		nxtDisplayString(4, "%8d", nMotorEncoder[M_DRIVE_BR]);
		nxtDisplayString(5, "%8d", nMotorEncoder[M_DRIVE_BL]);
		if (!BTN_IGNORE_ENC && ((enc_R_lift < 0 && js_lift < 0) || (enc_R_lift > enc_lift_max && js_lift > 0)))
			pwr_lift = 0;
		else
			pwr_lift = js_lift;


		motor[M_LIFT_L] = pwr_lift;
		motor[M_LIFT_R] = pwr_lift;
		int enc_close_lid = 6000; //where to close the bucket lid on the way up
		if (nMotorEncoder[M_LIFT_L] > (enc_close_lid+250) && nMotorEncoder[M_LIFT_L] < (enc_close_lid+250) && pwr_lift > 0) {
			servo[SV_LID] = 0; //close block stopper
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
		while (BTN_KILL) {
			haltAllMotors();
		}

		EndTimeSlice();
	}
}
