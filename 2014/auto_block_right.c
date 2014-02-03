#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IRsensor,       sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     M_DRIVE_BL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     M_DRIVE_FL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     M_BELT,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_LIFT_L,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     M_DRIVE_BR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     M_DRIVE_FR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C3_1,     M_LIFT_R,    tmotorTetrix, openLoop, encoder)
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

#define PWR_SCAN 30
#define TIME_SCAN 5000 //this should get the robot to the last lift at PWR_SCAN

task main() {
	waitForStart();
	ClearTimer(T1);
	int time_remaining = 0;
		motor[M_DRIVE_FL] = -30;
	motor[M_DRIVE_FR] = 30;
	motor[M_DRIVE_BR] = -30;
	motor[M_DRIVE_BL] = 27;
	while (time1[T1] < TIME_SCAN) { //should reach last bin
		if (SensorValue[IRsensor] == 5) { //we see the IR beacon
			time_remaining = TIME_SCAN - time1[T1];
			goLeft(0);
			//haltAllMotors();
			 //how much farther we have to get to last lift
			break;
		}
	}

	approachAndScore();
	wait1Msec(1000);
	StopAllTasks();
	ClearTimer(T1);
	while (time1[T1] < time_remaining + 400) {
		goLeft(2.5*PWR_SCAN);
	}

	StopAllTasks();
	//get on ramp
	goLeft(100);
	wait1Msec(500);

	goForward(100);
	wait1Msec(900);

	goRight(100);
	wait1Msec(2100);

	haltAllMotors();
}
