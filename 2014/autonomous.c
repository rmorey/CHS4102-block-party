#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTMotor,  none)
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
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "robot.h"

//because i always forget which is which
#define RIGHT 1
#define LEFT 2
#define ORANGE 3

#define PWR_SCAN 30
#define TIME_SCAN 5000

bool start_on_right;
bool do_ramp;
bool score_block;
bool wait;
int pressed;
int pwr_x = 0;
int pwr_y = 0;

int getButton() { //gets next button press
	do {
		pressed = nNxtButtonPressed;
	} while (pressed == -1);

	return pressed;
}
void approachAndScore() {
	// approach lift, score, retreat
	nMotorEncoder[M_LIFT_R] = 0;

	goForward(40);
	wait1Msec(900);
	haltAllMotors();
	wait1Msec(10);

	raiseLiftTime();

	wait1Msec(50);

	scoreBlocks();


	//while (nMotorEncoder[M_LIFT_R] < 16000) {
	//	motor[M_LIFT_L] = 100;
	//	motor[M_LIFT_R] = 100;
	//}
	//motor[M_LIFT_L] = 0;
	//motor[M_LIFT_R] = 0;
	//wait1Msec(50);

	//scoreBlocks();
	//wait1Msec(50);

	while (nMotorEncoder[M_LIFT_R] > 6000) {
		motor[M_LIFT_L] = -100;
		motor[M_LIFT_R] = -100;
	}
	motor[M_LIFT_L] = 0;
	motor[M_LIFT_R] = 0;

	wait1Msec(200);

	goBack(40);
	wait1Msec(900);

	haltAllMotors();
}


task main()
{
	nxtDisplayCenteredBigTextLine(0, "Which side?");
	nxtDisplayCenteredBigTextLine(7, "Left     Right");

	pressed = getButton();

	switch (pressed) {
	case RIGHT:
		start_on_right = true;
	case LEFT:
		start_on_right = false;
	default:
		StopAllTasks();
	}

	eraseDisplay();
	nxtDisplayCenteredBigTextLine(0, "Block, ramp, or both?");
	nxtDisplayCenteredBigTextLine(7, "Block Both Ramp");

	pressed = getButton();

	switch (pressed) {
	case RIGHT:
		do_ramp = true;
		score_block = false;
	case LEFT:
		do_ramp = false;
		score_block = true;
	case ORANGE:
		do_ramp = true;
		score_block = true;
	default:
		StopAllTasks();
	}

	eraseDisplay();
	nxtDisplayCenteredBigTextLine(0, "Wait for Start?");
	nxtDisplayCenteredBigTextLine(7, "Yes    No");

	pressed = getButton();

	switch (pressed) {
	case RIGHT:
		wait = false;
	case LEFT:
		wait = true;
	default:
		StopAllTasks();
	}

	eraseDisplay();
	nxtDisplayCenteredBigTextLine(0, "Ready?");
	nxtDisplayCenteredBigTextLine(7, "Go!");

	pressed = getButton(); // we don't really need the value of this press, we just wait for a press to start

	if (wait) waitForStart();
	ClearTimer(T1); //keeps track of autonomous period, 30000 ms long

	if (do_ramp && !score_block) {
		//just get on ramp
		goForward(100);
		wait1Msec(900);
		if (start_on_right) goLeft(100); else goRight(100);
		wait1Msec(2100);
		haltAllMotors();
	}

	if (score_block) {

		//find IR beacon
		ClearTimer(T2);
		int time_remaining = 0;
		if (start_on_right) pwr_x = -PWR_SCAN; else pwr_x = PWR_SCAN;
		drive(pwr_x, pwr_y);
		while (time1[T2] < TIME_SCAN) { //should reach last bin
			if (SensorValue[IR] == 5) { //we see the IR beacon
				time_remaining = TIME_SCAN - time1[T2];
				driveStop();
				break;
			}
			int err = 20 - SensorValue[SONAR]; //20cm is our target distance
			if (abs(err) > 10) {
				pwr_y = 0; //ignore if error is really big, we're probably doing something wrong
			}
			else {
				pwr_y = -2*err; //try to correct for error
			}
			drive(pwr_x, pwr_y);
		}

		//score block
		approachAndScore();
		wait1Msec(500);

		if (do_ramp) {

			//go remaining distance TODO: return on closest side
			if (start_on_right) goLeft(2.5*PWR_SCAN); else goRight(2.5*PWR_SCAN);
			wait1Msec(time_remaining / 2.5);
			driveStop();

			//get on ramp
			if (start_on_right) goLeft(100); else goRight(100);
			wait1Msec(500);

			goForward(100);
			wait1Msec(900);

			if (start_on_right) goRight(100); else goLeft(100);
			wait1Msec(2100);

			driveStop();
		}
	}
}
