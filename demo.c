#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     Sonar,          sensorSONAR)
#pragma config(Sensor, S4,     IRsensor,       sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     M_drive_BL,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     M_drive_FL,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     M_belt,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_slider_L,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     M_drive_FR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     M_drive_BR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C3_1,     M_slider_R,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S2_C3_2,     M_flag,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    S_score,              tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_2,    S_arm_R,              tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    S_hang_R,             tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    S_arm_L,              tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    S_hang_L,             tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)

#include "robot.h"

task main()
{
	bFloatDuringInactiveMotorPWM = false;
	motor[M_belt] = 100;
	wait1Msec(3000);
	motor[M_belt] = -100;
	wait1Msec(3000);
	motor[M_belt] = 0;
	wait1Msec(3000);
	motor[M_slider_L] = 25;
	motor[M_slider_R] = 25;
	wait1Msec(3100);
	motor[M_slider_L] = 0;
	motor[M_slider_R] = 0;
	wait1Msec(3000);
	scoreBlocks();
	wait1Msec(3000);
	motor[M_flag] = -100;
	wait1Msec(2000);
	motor[M_flag] = 0;
}
