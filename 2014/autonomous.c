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

#define PWR_SCAN 100

int scan_direction;
bool scored = false;
bool ramp;
bool block;
bool wait;
bool debug;
int button;
int pwr_x = 0;
int pwr_y = 0;
int ramp_direction;


void initializeRobot() {
	servo[SV_AUTO] = 0;
	servo[SV_HOOK] = 245;
	servo[SV_LID] = 255;
}

void getButton() { //gets next button press
	while (true) {
		button = nNxtButtonPressed;
		if (button != -1)
			break;
	}
	PlaySoundFile("! Click.rso");
	while(nNxtButtonPressed != -1) {
	}
}

void trackWall(int dist) {
	int err = dist - SensorValue[SONAR];
	if (abs(err) > 50)
		pwr_y = 0; //ignore if error is really big
	else
		pwr_y = -3*err; //try to correct for error
}


task main() {
	initializeRobot();
	//disableDiagnosticsDisplay();

	eraseDisplay();
	nxtDisplayCenteredBigTextLine(6, "L   R");
	getButton();

	switch (button) {
	case kRightButton:
		scan_direction = -1;
		break;
	case kLeftButton:
		scan_direction = 1;
		break;
	default:
		StopAllTasks();
	}

	eraseDisplay();
	nxtDisplayCenteredTextLine(4, "Both");
	nxtDisplayCenteredTextLine(6, "Block|Ramp");
	getButton();

	switch (button) {
	case kRightButton:
		ramp = true;
		block = false;
		break;
	case kLeftButton:
		ramp = false;
		block = true;
		break;
	case kEnterButton:
		ramp = true;
		block = true;
		break;
	default:
		StopAllTasks();
	}

	if (block && ramp) {

		eraseDisplay();
		nxtDisplayCenteredTextLine(4, "Ramp Side?");
		nxtDisplayCenteredTextLine(6, "Far|Closest|Near");
		getButton();

		switch (button) {
		case kRightButton: //near side
			ramp_direction = -scan_direction;
			break;
		case kLeftButton: //far side
			ramp_direction = scan_direction;
			break;
		case kEnterButton: //closest side based on encoders
			ramp_direction = 0;
			break;
		default:
			StopAllTasks();
		}
	}

	eraseDisplay();
	nxtDisplayCenteredBigTextLine(1, "Wait for Start?");
	nxtDisplayCenteredBigTextLine(6, "Yes    No");
	getButton();

	switch (button) {
	case kRightButton:
		wait = false;
		break;
	case kLeftButton:
		wait = true;
		break;
	default:
		StopAllTasks();
	}

	eraseDisplay();
	nxtDisplayCenteredBigTextLine(1, "Debug?");
	nxtDisplayCenteredBigTextLine(6, "Yes   No");
	getButton();

	switch (button) {
	case kRightButton:
		debug = false;
		break;
	case kLeftButton:
		debug = true;
		break;
	default:
		StopAllTasks();
	}

	//optional delay before starting autonomous
	eraseDisplay();
	nxtDisplayCenteredBigTextLine(1, "Delay?");
	nxtDisplayCenteredBigTextLine(6,"0");

	int delay = 0; //number of seconds to delay
	bool done = false;
	while(!done) {
		nxtDisplayCenteredBigTextLine(6," %d ",delay);
		getButton();
		switch (button) {
		case kRightButton:
			delay++;
			break;
		case kLeftButton:
			if (delay > 0)
				delay--;
			break;
		case kEnterButton:
			done = true;
			break;
		default:
			StopAllTasks();
		}
	}

	eraseDisplay();
	nxtDisplayCenteredBigTextLine(0, "Ready?");
	nxtDisplayCenteredBigTextLine(6, "Go!");
	getButton();

	switch (button) {
	case kEnterButton:
		break;
	default:
		StopAllTasks();
	}

	////////////////////////////////////////////////////
	//code that actually runs during match starts here//
	////////////////////////////////////////////////////

	eraseDisplay();
	StartTask(displayDiagnostics);

	if (wait)
		waitForStart();

	wait1Msec(delay*1000);


	//just get on ramp, dead reckoning

	if (ramp && !block) {

		goForward(100);
		wait1Msec(900);

		driveFast(scan_direction*100,0);
		wait1Msec(2100);

		driveStop();
	}

	//score block in IR goal
	else {
		pwr_x = scan_direction*PWR_SCAN;
		driveTilted(pwr_x, pwr_y);
		while (SensorValue[SONAR] > 200) {
			//wait for it to reach the wall
		}
		nMotorEncoder[M_DRIVE_FL] = 0;
		while (abs(nMotorEncoder[M_DRIVE_FL]) < 6500) {
			nxtDisplayString(3, "%8d     ", nMotorEncoder[M_DRIVE_FL]);
			//nxtDisplayBigStringAt(0,50, "IR:%d   ", SensorValue[IR]);

			if (SensorValue[IR] == 5 || abs(nMotorEncoder[M_DRIVE_FL]) > 6500) { //we see the IR beacon
				scored = true;
				wait1Msec(80);
				driveStop();
				//int near_edge = abs(nMotorEncoder[M_DRIVE_FL]);
				//while (SensorValue[IR] == 5){
				//	trackWall(35);
				//	driveTilted(pwr_x, pwr_y);
				//}
				//driveStop();
				//int far_edge = abs(nMotorEncoder[M_DRIVE_FL]);
				//while (abs(nMotorEncoder[M_DRIVE_FL]) > (near_edge + far_edge)/2) {
				//	trackWall(35);
				//	driveTilted(-pwr_x, pwr_y);
				//}
				//driveStop();
				nxtDisplayString(3, "%8d     ", nMotorEncoder[M_DRIVE_FL]);

				if (ramp_direction == 0) { //if we have not already chosen a direction for the ramp
					if (abs(nMotorEncoder[M_DRIVE_FL]) > 2000) //TODO: calibrate this value
						ramp_direction = scan_direction;
					else
						ramp_direction = -scan_direction;
				}

				pwr_x = 0;
				pwr_y = 75;
				while(true) {
					int ir = SensorValue[IR];
					switch (ir) {
					case 4:
						pwr_x = -30;
						pwr_y = 0;
						break;
					case 6:
						pwr_x = 30;
						pwr_y = 0;
						break;
					default:
						pwr_x = 0;
						pwr_y = 50;
						break;
					}

					if (SensorValue[SONAR] < 30){
						wait1Msec(100);
						if (SensorValue[SONAR] < 30)
							break;
					}

					driveTilted(pwr_x,pwr_y);
				}

				driveStop();

				if (debug)
					wait1Msec(1000);

				servo[SV_AUTO] = 130; //score the autonomous block
				wait1Msec(400);
				servo[SV_AUTO] = 24;
				wait1Msec(100);

				if (debug)
					wait1Msec(1000);

				break;
			}
			trackWall(35);
			driveTilted(pwr_x, pwr_y);
		}

		driveStop();

		if (ramp) {
			//go remaining distance

			pwr_x = ramp_direction*1.5*PWR_SCAN;
			pwr_y = -10; //get a bit away from baskets
			driveTilted(pwr_x, pwr_y);
			while(SensorValue[SONAR] < 200) {
				trackWall(45);
				driveTilted(pwr_x, pwr_y);
			}
			servo[SV_AUTO] = 0;
			driveStop();

			if (debug)
				wait1Msec(1000);

			//get on ramp

			//get away from baskets
			if (ramp_direction == 1)
				goForward(100);
			else
				goLeft(100);
			wait1Msec(1000);
			driveStop();

			if (debug)
				wait1Msec(1000);

			//get in front of ramp
			goForwardLeft(100);
			wait1Msec(1000);
			driveStop();

			if (debug)
				wait1Msec(1000);

			//rotate to face ramp
			rotate(-100);
			wait1Msec(350);
			driveStop();

			if (debug)
				wait1Msec(1000);

			//get on ramp
			if (ramp_direction == 1)
				goLeft(100);
			else
				goRight(100);
			wait1Msec(2000);
			driveStop();

		}
	}
}
