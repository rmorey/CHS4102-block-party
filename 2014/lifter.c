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
//lifter controls directly from the nxt

#include "JoystickDriver.c"
task main()
{
	while(true) {
		while (nNxtButtonPressed == kRightButton) {
			motor[M_LIFT_R] = 100;
			motor[M_LIFT_L] = 100;
		}
		while (nNxtButtonPressed == kLeftButton) {
			motor[M_LIFT_R] = -100;
			motor[M_LIFT_L] = -100;
		}
		motor[M_LIFT_R] = 0;
		motor[M_LIFT_L] = 0;
	}
}
