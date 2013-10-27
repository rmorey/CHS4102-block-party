#include "motors_10.27.13.h" // this has all the #pragma statements
#include "JoystickDriver.c"
#include "buttons_10.27.13.h" // this has all the button definitions

#define NUDGE_SPEED 15
#define BUCKET_SPEED 20   
#define CONVEYOR_SPEED 100
#define SLIDER_SPEED 10

void InitializeRobot (){
  return;
}

task main() {
  InitializeRobot();
  int conveyor_status = 0;
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
    motor[motorFR] = FRpower;
    motor[motorFL] = FLpower;
    motor[motorBR] = BRpower;
    motor[motorBL] = BLpower;
    
    //dpad nudging
    switch(DRIVE_DPAD) {
    case -1: //not pressed
      break;
    case 0: //forward
      motor[motorFR] = NUDGE_SPEED; // we may have to tweak these to get consistent speed
      motor[motorFL] = NUDGE_SPEED;
      motor[motorBR] = NUDGE_SPEED;
      motor[motorBL] = NUDGE_SPEED;
      wait1Msec(20);
      break;
    case 1: //forward-right
      motor[motorFR] = 0;
      motor[motorFL] = NUDGE_SPEED;
      motor[motorBR] = NUDGE_SPEED;
      motor[motorBL] = 0;
      wait1Msec(20);
      break;
    case 2: //right
      motor[motorFR] = -NUDGE_SPEED;
      motor[motorFL] = NUDGE_SPEED;
      motor[motorBR] = NUDGE_SPEED;
      motor[motorBL] = -NUDGE_SPEED;
      wait1Msec(20);
      break;
    case 3: // back-right
      motor[motorFR] = -NUDGE_SPEED;
      motor[motorFL] = 0;
      motor[motorBR] = 0;
      motor[motorBL] = -NUDGE_SPEED;
      wait1Msec(20);
      break;
    case 4: // backward
      motor[motorFR] = -NUDGE_SPEED;
      motor[motorFL] = -NUDGE_SPEED;
      motor[motorBR] = -NUDGE_SPEED;
      motor[motorBL] = -NUDGE_SPEED;
      wait1Msec(20);
      break;
    case 5: //back-left
      motor[motorFR] = 0;
      motor[motorFL] = -NUDGE_SPEED;
      motor[motorBR] = -NUDGE_SPEED;
      motor[motorBL] = 0;
      wait1Msec(20);
      break;
    case 6: // left
      motor[motorFR] = NUDGE_SPEED;
      motor[motorFL] = -NUDGE_SPEED;
      motor[motorBR] = -NUDGE_SPEED;
      motor[motorBL] = NUDGE_SPEED;
      wait1Msec(20);
      break;
    case 7: //forward-left
      motor[motorFR] = NUDGE_SPEED;
      motor[motorFL] = 0;
      motor[motorBR] = 0;
      motor[motorBL] = NUDGE_SPEED;
      wait1Msec(20);
      break;
    }
    
    //joystick bucket control
    motor[motorLB] = (50*BUCKET_JS)/127;
    motor[motorRB] = (50*BUCKET_JS)/127;

    //dpad bucket control
    switch(BUCKET_DPAD) {
    default:
      break;
    case 0:
      motor[motorLB] = BUCKET_SPEED;  
      motor[motorRB] = BUCKET_SPEED;
      while(BUCKET_DPAD == 0) {
	getJoystickSettings(joystick); // maintains speed while button is being pressed
      }
      motor[motorLB] = 0; // stops once button is no longer pressed
      motor[motorRB] = 0;
      break;
    case 4:
      motor[motorLB] = -BUCKET_SPEED;
      motor[motorRB] = -BUCKET_SPEED;
      while(BUCKET_DPAD == 4) {
	getJoystickSettings(joystick);
      }
      motor[motorLB] = 0;
      motor[motorRB] = 0;
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
    motor[motorBELT] = conveyor_status * CONVEYOR_SPEED;
    
    // uses triggers to control sliders individually 
    
    if (LEFT_SLIDER_UP_BTN) {
      motor[motorLB] = SLIDER_SPEED;
      while (LEFT_SLIDER_UP_BTN) {
	getJoystickSettings(joystick);
      }
      motor[motorLB] = 0;
    }
    if (LEFT_SLIDER_DOWN_BTN) {
      motor[motorLB] = -SLIDER_SPEED;
      while (LEFT_SLIDER_DOWN_BTN) {
	getJoystickSettings(joystick);
      }
      motor[motorLB] = 0;
    }
    if (RIGHT_SLIDER_UP_BTN) {
      motor[motorRB] = SLIDER_SPEED;
      while (RIGHT_SLIDER_UP_BTN) {
	getJoystickSettings(joystick);
      }
      motor[motorRB] = 0;
    }
    if (RIGHT_SLIDER_DOWN_BTN) {
      motor[motorRB] = -SLIDER_SPEED;
      while (RIGHT_SLIDER_DOWN_BTN) {
	getJoystickSettings(joystick);
      }
      motor[motorRB] = 0;
    }
  }
}  
