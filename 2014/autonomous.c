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
#pragma config(Servo,  srvo_S2_C1_4,    S_AUTO,               tServoStandard)
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
#define ENC_LAST_GOAL 10000 //distance to last goal

bool start_on_right;
bool ramp;
bool block;
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

task main()
{
	eraseDisplay();
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
		ramp = true;
		block = false;
	case LEFT:
		ramp = false;
		block = true;
	case ORANGE:
		ramp = true;
		block = true;
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

	/////////////////////////////////////////////////////////
	////code that actually runs during match starts here/////
	/////////////////////////////////////////////////////////
	if (wait) waitForStart();

	ClearTimer(T1); //keeps track of autonomous period, 30000 ms long

	if (ramp && !block) {
		//just get on ramp
		goForward(100);
		wait1Msec(900);
		if (start_on_right) goLeft(100); else goRight(100);
		wait1Msec(2100);
		driveStop();
	}

	else {
		//score block in IR goal
		if (start_on_right) pwr_x = -PWR_SCAN; else pwr_x = PWR_SCAN;
		driveTilted(pwr_x, pwr_y);
		while (SensorValue[SONAR] < 50) { //should reach last bin
			if (SensorValue[IR] == 5) { //we see the IR beacon
				driveStop();
				//score block here
				break;
			}
			int err = 10 - SensorValue[SONAR]; //10cm is our target distance
			if (abs(err) > 10) {
				pwr_y = 0; //ignore if error is really big, we're probably doing something wrong
			}
			else {
				pwr_y = -2*err; //try to correct for error, maybe change this value
			}
			driveTilted(pwr_x, pwr_y);
		}

		if (ramp) {

			//go remaining distance TODO: return on closest side
			if (start_on_right) pwr_x = -2.5*PWR_SCAN; else pwr_x = 2.5*PWR_SCAN;
			driveTilted(pwr_x, pwr_y);
			while(SensorValue[SONAR] < 50) {
				int err = 10 - SensorValue[SONAR]; //10cm is our target distance
				if (abs(err) > 10) {
					pwr_y = 0; //ignore if error is really big, we're probably doing something wrong
				}
				else {
					pwr_y = -2*err; //try to correct for error, maybe change this value
				}
				driveTilted(pwr_x, pwr_y);
			}
			driveStop();

			//get on ramp

			goForward(100);
			wait1Msec(900);

			if (start_on_right) goRight(100); else goLeft(100);
			wait1Msec(2100);

			driveStop();
		}
	}
}
