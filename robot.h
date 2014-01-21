/*
A few conventions:
BTN/btn denotes a controller button, returns 1 if pressed, 0 otherwise (actually maybe they return booleans...)
JS/js denotes a joystick coordinate, an int in (-127,127), remember that pushing up on the js returns a negative y-coord
ENC/enc denotes an encoder value, an int, 1440 ticks to a rotation (dont ask me why...)
PWR/pwr denotes a motor power as an int in (-100,100)

ALL CAPS means a constant
lowerCase means a variable
*/

//driver controls 
#define BTN_FLAG joy1Btn(1) //hold to spin flag	
#define BTN_TILT joy1Btn(2) //toggles drive mode
#define BTN_BELT_IN joy1Btn(5) //hold to pull blocks in
#define BTN_BELT_OUT joy1Btn(6) //hold to push blocks out 
#define JS_DRIVE_X joystick.joy1_x1 
#define JS_DRIVE_Y joystick.joy1_y1
#define JS_ROTATE joystick.joy1_x2

//scorer controls
#define BTN_SCORE joy2Btn(1) //scores blocks
#define BTN_ENC_RESET joy2Btn(2) //reset the bucket encoder values to zero
#define JS_BUCKET joystick.joy2_y1

//both drivers
#define BTN_KILL (joy1Btn(10) || joy2Btn(10))

//encoder defs
#define ENC_BUCKET_MAX 9001 //TODO: figure this out

//go in the specified direction at the specified power
void goForward (int pwr) {
	motor[M_drive_FL] = pwr;
	motor[M_drive_FR] = pwr;
	motor[M_drive_BR] = pwr;
	motor[M_drive_BL] = pwr;
}
void goBack (int pwr) {
	goForward(-pwr);
}
void goRight (int pwr) {
	motor[M_drive_FL] = pwr;
	motor[M_drive_FR] = pwr;
	motor[M_drive_BR] = -pwr;
	motor[M_drive_BL] = -pwr;
}
void goLeft (int pwr) {
	goRight(-pwr);
}
void goForwardRight (int pwr) {
	motor[M_drive_FL] = pwr;
	motor[M_drive_FR] = 0;
	motor[M_drive_BR] = pwr;
	motor[M_drive_BL] = 0;
}
void goBackLeft (int pwr) {
	goForwardRight(-pwr);
}
void goForwardLeft (int pwr) {
	motor[M_drive_FL] = 0;
	motor[M_drive_FR] = pwr;
	motor[M_drive_BR] = 0;
	motor[M_drive_BL] = pwr;
}
void goBackRight (int pwr) {
	goForwardLeft(-pwr);
}

//stops all motors
void haltAllMotors () { 
	motor[M_drive_FL] = 0;
	motor[M_drive_FR] = 0;
	motor[M_drive_BR] = 0;
	motor[M_drive_BL] = 0;
	motor[M_slider_R] = 0;
	motor[M_slider_L] = 0;
	motor[M_flag] = 0;
	motor[M_belt] = 0;
}

//raise and lower bucket using time, kinda sketch..
void raiseBucket() {
	motor[M_slider_L] = 25;
	motor[M_slider_R] = 25;
	wait1Msec(2100);
	motor[M_slider_L] = 0;
	motor[M_slider_R] = 0;
}

void lowerBucket() {
	motor[M_slider_L] = -25;
	motor[M_slider_R] = -25;
	wait1Msec(1600);
	motor[M_slider_L] = 0;
	motor[M_slider_R] = 0;
}

//pushes blocks out and brings pusher back in
void scoreBlocks() {
	servo[S_score] = 0;
	wait1Msec(1400);
	servo[S_score] = 255;
	wait1Msec(1400);
	servo[S_score] = 128;
}
