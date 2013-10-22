#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorFL,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorBL,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorBR,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     motorFR,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     motorLB,       tmotorTetrix, openLoop, reversed) // left slider
#pragma config(Motor,  mtr_S2_C2_1,     motorRB,       tmotorTetrix, openLoop) //right slider
#pragma config(Motor,  mtr_S2_C2_2,     motorK,        tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

#define SCORE_BTN joy2Btn(1)
#define UNSCORE_BTN joy2Btn(2)
#define CONVEYOR_IN_BTN joy2Btn(3)
#define CONVEYOR_OUT_BTN joy2Btn(4)
#define SLOW_ROTATE_CW_BTN joy1Btn(6) // cw means clockwise, ccw means counter-clockwise
#define SLOW_ROTATE_CCW_BTN joy1Btn(5) 
#define FAST_ROTATE_CW_BTN joy1Btn(8)
#define FAST_ROTATE_CCW_BTN joy1Btn(7)
#define FLAGGER_OUT_BTN joy2Btn(6)
#define FLAGGER_IN_BTN joy2Btn(5)
#define SPIN_FLAGGER_CW_BTN joy2Btn(8)
#define SPIN_FLAGGER_CCW_BTN joy2Btn(7)
#define CATCH_BTN joy2Btn(9)
#define RELEASE_BTN joy2Btn(10)
#define DRIVE_JS_X joystick.joy1_x1
#define DRIVE_JS_Y joystick.joy1_y1
#define DRIVE_DPAD joystick.joy1_TopHat
#define ROTATE_JS joystick.joy1_x2
#define BUCKET_JS joystick.joy2_y1
#define BUCKET_DPAD joystick.joy2_TopHat


#define SLOW_SPEED 20
#define FAST_SPEED 70

void InitializeRobot (){
	return;
}

task main(){
	InitializeRobot();
	waitForStart();

	while(true) {
	//joystick drive
	motor[motorFR] = (70.7*DRIVE_JS_X)/127.0-(70.7*DRIVE_JS_Y)/127.0;
	motor[motorFL] = (70.7*DRIVE_JS_Y)/127.0+(70.7*DRIVE_JS_X)/127.0;
	motor[motorBR] = (70.7*DRIVE_JS_Y)/127.0+(70.7*DRIVE_JS_X)/127.0;
	motor[motorBL] = (70.7*DRIVE_JS_X)/127.0-(70.7*DRIVE_JS_Y)/127.0;
	
	//slow dpad drive
	switch(DRIVE_DPAD) {
		case -1: //not pressed
			break;
		case 0: //forward
			motor[motorFR] = SLOW_SPEED; // we may have to tweak these to get consistent speed
			motor[motorFL] = SLOW_SPEED;
			motor[motorBR] = SLOW_SPEED;
			motor[motorBL] = SLOW_SPEED;
			break;
		case 1: //forward-right
			motor[motorFR] = 0;
			motor[motorFL] = SLOW_SPEED;
			motor[motorBR] = SLOW_SPEED;
			motor[motorBL] = 0;
			break;
		case 2: //right
			motor[motorFR] = -SLOW_SPEED;
			motor[motorFL] = SLOW_SPEED;
			motor[motorBR] = SLOW_SPEED;
			motor[motorBL] = -SLOW_SPEED;
			break;
		case 3: // back-right
			motor[motorFR] = -SLOW_SPEED;
			motor[motorFL] = 0;
			motor[motorBR] = 0;
			motor[motorBL] = -SLOW_SPEED;
			break;
		case 4: // backward
			motor[motorFR] = -SLOW_SPEED;
			motor[motorFL] = -SLOW_SPEED;
			motor[motorBR] = -SLOW_SPEED;
			motor[motorBL] = -SLOW_SPEED;
			break;
		case 5: //back-left
			motor[motorFR] = 0;
			motor[motorFL] = -SLOW_SPEED;
			motor[motorBR] = -SLOW_SPEED;
			motor[motorBL] = 0;
			break;
		case 6: // left
			motor[motorFR] = SLOW_SPEED;
			motor[motorFL] = -SLOW_SPEED;
			motor[motorBR] = -SLOW_SPEED;
			motor[motorBL] = SLOW_SPEED;
			break;
		case 7: //forward-left
			motor[motorFR] = SLOW_SPEED;
			motor[motorFL] = 0;
			motor[motorBR] = 0;
			motor[motorBL] = SLOW_SPEED;
			break;
		}
		
	//joystick rotate (only uses x value)
	motor[motorFR] = -(100*ROTATE_JS)/127;
	motor[motorFL] = (100*ROTATE_JS)/127;
	motor[motorBR] = -(100*ROTATE_JS)/127;
	motor[motorBL] = (100*ROTATE_JS)/127;
	
	//slow trigger rotate
	if(SLOW_ROTATE_CW_BTN){
		motor[motorFR] = -SLOW_SPEED;
		motor[motorFL] = SLOW_SPEED;
		motor[motorBR] = -SLOW_SPEED;
		motor[motorBL] = SLOW_SPEED;
	}
	if(SLOW_ROTATE_CCW_BTN) {
		motor[motorFR] = SLOW_SPEED;
		motor[motorFL] = -SLOW_SPEED;
		motor[motorBR] = SLOW_SPEED;
		motor[motorBL] = -SLOW_SPEED;
	}
	
	//fast trigger rotate
	if(FAST_ROTATE_CW_BTN){
		motor[motorFR] = -FAST_SPEED;
		motor[motorFL] = FAST_SPEED;
		motor[motorBR] = -FAST_SPEED;
		motor[motorBL] = FAST_SPEED;
	}
	if(FAST_ROTATE_CCW_BTN) {
		motor[motorFR] = FAST_SPEED;
		motor[motorFL] = -FAST_SPEED;
		motor[motorBR] = FAST_SPEED;
		motor[motorBL] = -FAST_SPEED;
	}	
	
	// joystick bucket control (fast)
	motor[motorLB] = (100*BUCKET_JS)/127;
	motor[motorRB] = (100*BUCKET_JS)/127;
	
	// dpad bucket control (slow)
	switch(
	
	// scores/unscores , only moves left slider
	
	//conveyor belt control
	
	//flagger in/out
	
	//spin flagger
}
}