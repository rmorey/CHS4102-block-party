#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IRsensor,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     ColorSensor,    sensorI2CHiTechnicColor)
#pragma config(Motor,  mtr_S1_C1_1,     M_drive_BR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     M_drive_BL,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     M_drive_FL,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     M_drive_FR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     M_slider_R,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     M_slider_L,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     M_belt,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     M_flag,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    S_score_1,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    S_score_2,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "buttons.h" // this has all the button definitions

#define NUDGE_POWER 15
#define BUCKET_SPEED 20
#define SLIDER_SPEED 10

void InitializeRobot (){
	nMotorEncoder[M_slider_R] = 0;
	nMotorEncoder[M_slider_L] = 0;
	servo[S_score_1] = 9001; //TODO fix this
	servo[S_score_2] = 9001;
	return;
}

task main() {
	InitializeRobot();
	int conveyor_status = 0; // because the conveyor is not currently moving
	waitForStart();
	while(true) {
		getJoystickSettings(joystick);

		//uses joystick values and rotation matrix to get wheel powers
		int FRpower = (70.7*DRIVE_JS_X)/127.0-(70.7*DRIVE_JS_Y)/127.0 + (100*ROTATE_JS)/127;
		int FLpower = (70.7*DRIVE_JS_Y)/127.0+(70.7*DRIVE_JS_X)/127.0 + (100*ROTATE_JS)/127;
		int BRpower = (70.7*DRIVE_JS_Y)/127.0+(70.7*DRIVE_JS_X)/127.0 - (100*ROTATE_JS)/127;
		int BLpower = (70.7*DRIVE_JS_X)/127.0-(70.7*DRIVE_JS_Y)/127.0 - (100*ROTATE_JS)/127;

		//slippage control, if |power|<10, it's treated as 0
		if (abs(FRpower) < 10) {
			FRpower = 0;
		}
		if (abs(FLpower) < 10) {
			FLpower = 0;
		}
		if (abs(BRpower) < 10) {
			BRpower = 0;
		}
		if (abs(BLpower) < 10) {
			BLpower = 0;
		}

		//assigns power to motors
		motor[M_drive_FR] = FRpower;
		motor[M_drive_FL] = FLpower;
		motor[M_drive_BR] = BRpower;
		motor[M_drive_BL] = BLpower;

		//dpad nudging
		switch(DRIVE_DPAD) {
		case -1: //not pressed
			break;
		case 0: //forward
			goForward(NUDGE_POWER);
			wait1Msec(20);
			break;
		case 1: //forward-right
			goForwardRight(NUDGE_POWER);
			wait1Msec(20);
			break;
		case 2: //right
			goRight(NUDGE_POWER);
			wait1Msec(20);
			break;
		case 3: // back-right
			goBackRight(NUDGE_POWER);
			wait1Msec(20);
			break;
		case 4: // backward
			goBack(NUDGE_POWER);
			wait1Msec(20);
			break;
		case 5: //back-left
			goBackLeft(NUDGE_POWER);
			wait1Msec(20);
			break;
		case 6: // left
			goLeft(NUDGE_POWER);
			wait1Msec(20);
			break;
		case 7: //forward-left
			goForwardLeft(NUDGE_POWER);
			wait1Msec(20);
			break;
		}

		//joystick bucket control
		motor[M_slider_L] = (50*BUCKET_JS)/127;
		motor[M_slider_R] = (50*BUCKET_JS)/127;

		//dpad bucket control
		switch(BUCKET_DPAD) {
		default:
			break;
		case 0:
			motor[M_slider_L] = BUCKET_SPEED;
			motor[M_slider_R] = BUCKET_SPEED;
			while(BUCKET_DPAD == 0) {
				getJoystickSettings(joystick); // maintains speed while button is being pressed
			}
			motor[M_slider_L] = 0; // stops once button is no longer pressed
			motor[M_slider_R] = 0;
			break;
		case 4:
			motor[M_slider_L] = -BUCKET_SPEED;
			motor[M_slider_R] = -BUCKET_SPEED;
			while(BUCKET_DPAD == 4) {
				getJoystickSettings(joystick);
			}
			motor[M_slider_L] = 0;
			motor[M_slider_R] = 0;
		}

		//conveyor belt control
		if(CONVEYOR_IN_BTN) {
			if (conveyor_status == 1) { //if belt is already moving in that direction, then stop it
				conveyor_status = 0;
			}
			else { // otherwise, make it move in that direction
				conveyor_status = 1;
			}
		}

		if(CONVEYOR_OUT_BTN) {
			if (conveyor_status == -1)	{
				conveyor_status = 0;
			}
			else {
				conveyor_status = -1;
			}
		}
		motor[M_belt] = conveyor_status * 100;

		// uses triggers to control sliders individually

		if (LEFT_SLIDER_UP_BTN) {
			motor[M_slider_L] = SLIDER_SPEED;
			while (LEFT_SLIDER_UP_BTN) {
				getJoystickSettings(joystick);
			}
			motor[M_slider_L] = 0;
		}
		if (LEFT_SLIDER_DOWN_BTN) {
			motor[M_slider_L] = -SLIDER_SPEED;
			while (LEFT_SLIDER_DOWN_BTN) {
				getJoystickSettings(joystick);
			}
			motor[M_slider_L] = 0;
		}
		if (RIGHT_SLIDER_UP_BTN) {
			motor[M_slider_R] = SLIDER_SPEED;
			while (RIGHT_SLIDER_UP_BTN) {
				getJoystickSettings(joystick);
			}
			motor[M_slider_R] = 0;
		}
		if (RIGHT_SLIDER_DOWN_BTN) {
			motor[M_slider_R] = -SLIDER_SPEED;
			while (RIGHT_SLIDER_DOWN_BTN) {
				getJoystickSettings(joystick);
			}
			motor[M_slider_R] = 0;
		}

		if (SPIN_FLAGGER_CW_BTN) {
			motor[M_flag] = 100;
			while(SPIN_FLAGGER_CW_BTN)
			{
				getJoystickSettings(joystick);
			}
			motor[M_flag] = 0;
		}
		if (SPIN_FLAGGER_CCW_BTN) {
			motor[M_flag] = -100;
			while (SPIN_FLAGGER_CCW_BTN) {
				getJoystickSettings(joystick);
			}
			motor[M_flag] = 0;
		}

		if (SCORE_BTN) {
			scoreBlocks();
		}

		if (RESET_BTN) {
			resetBucket();
		}

	}
}