#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IRsensor,       sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     M_drive_BL,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     M_drive_FL,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     M_belt,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_slider_L,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     M_drive_BR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     M_drive_FR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C3_1,     M_slider_R,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C3_2,     M_flag,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    S_score,              tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_2,    S_hook,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include fil\e to "handle" the Bluetooth messages.
#include "robot.h"

void initializeRobot() {
	bFloatDuringInactiveMotorPWM = false; //motors will brake when stopped\
	return;
}

task main()
{
	initializeRobot();
	waitForStart();

	goForward(100);
	wait1Msec(900);

	goLeft(100);
	wait1Msec(2100);

	haltAllMotors();
}
