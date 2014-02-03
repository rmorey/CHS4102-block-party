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

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "robot.h"

#define SPEED 50

void initializeRobot() {
	bFloatDuringInactiveMotorPWM = false; //motors will brake when stopped\
	return;
}

task main()
{
	initializeRobot();
	//waitForStart();

	//go right to get in front of basket, raise bucke
	goLeft(SPEED);
	wait1Msec(1000); //TODO: joe check this
	haltAllMotors();
	wait1Msec(70);


	//approach basket
	goForward(SPEED);
	wait1Msec(600); //TODO: joe check this
	haltAllMotors();
	wait1Msec(70);

	raiseLift();
	wait1Msec(70);

	//scores blocks, resets pusher
	scoreBlocks();

	//backup
	goBack(SPEED);
	wait1Msec(600); //TODO: joe check this

	//get up on the ramp
	goLeft(SPEED);
	wait1Msec(2000); //TODO: joe check this

	//goForward(SPEED);
	//wait1Msec(1650); //TODO: joe check this

	//goRight(SPEED);
	//wait1Msec(4000); //TODO: joe check this

	haltAllMotors();

}
