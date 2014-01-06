
//driver
#define FLAG_BTN joy1Btn(1)
#define TILT_BTN joy1Btn(2)
#define BELT_IN_BTN joy1Btn(5)
#define BELT_OUT_BTN joy1Btn(6)

//scorer
#define SCORE_BTN joy2Btn(1)
#define KILL_BTN (joy1Btn(10) || joy2Btn(10))

//joysticks
#define DRIVE_JS_X joystick.joy1_x1
#define DRIVE_JS_Y joystick.joy1_y1
#define ROTATE_JS joystick.joy1_x2
#define BUCKET_JS joystick.joy2_y1

void goForward (int power) {// these functions go in the specified direction at the specified power
	motor[M_drive_FL] = power;
	motor[M_drive_FR] = power;
	motor[M_drive_BR] = power;
	motor[M_drive_BL] = power;
}
void goBack (int power) {
	goForward(-power);
}
void goRight (int power) {
	motor[M_drive_FL] = power;
	motor[M_drive_FR] = power;
	motor[M_drive_BR] = -power;
	motor[M_drive_BL] = -power;
}
void goLeft (int power) {
	goRight(-power);
}
void goForwardRight (int power) {
	motor[M_drive_FL] = power;
	motor[M_drive_FR] = 0;
	motor[M_drive_BR] = power;
	motor[M_drive_BL] = 0;
}
void goBackLeft (int power) {
	goForwardRight(-power);
}
void goForwardLeft (int power) {
	motor[M_drive_FL] = 0;
	motor[M_drive_FR] = power;
	motor[M_drive_BR] = 0;
	motor[M_drive_BL] = power;
}
void goBackRight (int power) {
	goForwardLeft(-power);
}

void haltAllMotors () { //stops all motors
	motor[M_drive_FL] = 0;
	motor[M_drive_FR] = 0;
	motor[M_drive_BR] = 0;
	motor[M_drive_BL] = 0;
	motor[M_slider_R] = 0;
	motor[M_slider_L] = 0;
	motor[M_flag] = 0;
	motor[M_belt] = 0;
}

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

void scoreBlocks() { //pushes blocks out and brings pusher back in
	servo[S_score] = 0;
	wait1Msec(1400);
	servo[S_score] = 255;
	wait1Msec(1400);
	servo[S_score] = 128;
}
