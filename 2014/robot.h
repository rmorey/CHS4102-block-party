/*
A few conventions:
BTN/btn denotes a controller button, returns 1 if pressed, 0 otherwise (actually maybe they return booleans...)
JS/js denotes a joystick coordinate, an int in (-127,127), remember that pushing up on the js returns a negative y-coord
ENC/enc denotes an encoder value, an int, 1440 ticks to a rotation (dont ask me why...)
PWR/pwr denotes a motor power as an int in (-100,100)
M denotes motor
S is a servo

ALL CAPS means a constant
lowerCase means a variable
*/

//driver controls
#define BTN_FLAG joy1Btn(1) //hold to spin flag
#define BTN_TILT joy1Btn(2) //toggles drive mode
#define BTN_BACK joy1Btn(3) //backwards drive mode
#define BTN_FRONT joy1Btn(4) //revert to standard driving mode
#define BTN_BELT_IN joy1Btn(5) //hold to pull blocks in
#define BTN_BELT_OUT joy1Btn(6) //hold to push blocks out
#define JS_DRIVE_X joystick.joy1_x1 //directional driving
#define JS_DRIVE_Y joystick.joy1_y1 //directional driving
#define JS_ROTATE joystick.joy1_x2 //rotation

//scorer controls
#define BTN_SCORE joy2Btn(1) //scores blocks
#define BTN_LID joy2Btn(2) //toggles thing that keeps blocks in
#define BTN_HOOK_OUT joy2Btn(3) // hook control
#define BTN_HOOK_IN joy2Btn(4) //hook control
#define BTN_IGNORE_ENC joy2Btn(5) // ignores lifter encoder limits when held
#define BTN_ZERO_LIFT joy2Btn(6) //reset the lift encoder values to zero
#define BTN_SETMAX_LIFT joy2Btn(8) //sets max lift height
#define JS_LIFT joystick.joy2_y1 //controls lift

//both drivers
#define BTN_KILL (joy1Btn(10) || joy2Btn(10))

//encoder defs
#define ENC_LIFT_MAX 17000 //TODO: figure this out

//go in the specified direction at the specified power

void goDirection(int pwr_x, int pwr_y) {
}
void goForward (int pwr) {
	motor[M_DRIVE_FL] = pwr;
	motor[M_DRIVE_FR] = pwr;
	motor[M_DRIVE_BR] = pwr;
	motor[M_DRIVE_BL] = pwr;
}
void goBack (int pwr) {
	goForward(-pwr);
}
void goRight (int pwr) {
	motor[M_DRIVE_FL] = pwr;
	motor[M_DRIVE_FR] = -pwr;
	motor[M_DRIVE_BR] = pwr;
	motor[M_DRIVE_BL] = -pwr;
}
void goLeft (int pwr) {
	goRight(-pwr);
}
void goForwardRight (int pwr) {
	motor[M_DRIVE_FL] = pwr;
	motor[M_DRIVE_FR] = 0;
	motor[M_DRIVE_BR] = pwr;
	motor[M_DRIVE_BL] = 0;
}
void goBackLeft (int pwr) {
	goForwardRight(-pwr);
}
void goForwardLeft (int pwr) {
	motor[M_DRIVE_FL] = 0;
	motor[M_DRIVE_FR] = pwr;
	motor[M_DRIVE_BR] = 0;
	motor[M_DRIVE_BL] = pwr;
}
void goBackRight (int pwr) {
	goForwardLeft(-pwr);
}

//stops all motors
void haltAllMotors () {
	motor[M_DRIVE_FL] = 0;
	motor[M_DRIVE_FR] = 0;
	motor[M_DRIVE_BR] = 0;
	motor[M_DRIVE_BL] = 0;
	motor[M_LIFT_R] = 0;
	motor[M_LIFT_L] = 0;
	motor[M_FLAG] = 0;
	motor[M_BELT] = 0;
}


void raiseLift() {
	while (nMotorEncoder[M_LIFT_R] < ENC_LIFT_MAX) {
		motor[M_LIFT_L] = 100;
		motor[M_LIFT_R] = 100;
	}
	motor[M_LIFT_L] = 0;
	motor[M_LIFT_R] = 0;
}

void lowerLift() {
	while (nMotorEncoder[M_LIFT_R] > 0) {
		motor[M_LIFT_L] = -100;
		motor[M_LIFT_R] = -100;
	}
	motor[M_LIFT_L] = 0;
	motor[M_LIFT_R] = 0;
}


//raise and lower lift using time, kinda sketch..
void raiseLiftTime() {
	motor[M_LIFT_L] = 100;
	motor[M_LIFT_R] = 100;
	wait1Msec(6000);
	motor[M_LIFT_L] = 0;
	motor[M_LIFT_R] = 0;
}

void lowerLiftTime() {
	motor[M_LIFT_L] = -25;
	motor[M_LIFT_R] = -25;
	wait1Msec(9000);
	motor[M_LIFT_L] = 0;
	motor[M_LIFT_R] = 0;
}

//pushes blocks out and brings pusher back in
void scoreBlocks() {
	servo[S_SCORE] = 0;
	wait1Msec(1400);
	servo[S_SCORE] = 255;
	wait1Msec(1400);
	servo[S_SCORE] = 128;
}

void approachAndScore() {
	// approach lift, score, retreat
	nMotorEncoder[M_LIFT_R] = 0;

	goForward(40);
	wait1Msec(900);
	haltAllMotors();
	wait1Msec(10);

	raiseLiftTime();

	wait1Msec(50);

	scoreBlocks();


	//while (nMotorEncoder[M_LIFT_R] < 16000) {
	//	motor[M_LIFT_L] = 100;
	//	motor[M_LIFT_R] = 100;
	//}
	//motor[M_LIFT_L] = 0;
	//motor[M_LIFT_R] = 0;
	//wait1Msec(50);

	//scoreBlocks();
	//wait1Msec(50);

	while (nMotorEncoder[M_LIFT_R] > 6000) {
		motor[M_LIFT_L] = -100;
		motor[M_LIFT_R] = -100;
	}
	motor[M_LIFT_L] = 0;
	motor[M_LIFT_R] = 0;

	wait1Msec(200);

	goBack(40);
	wait1Msec(900);

	haltAllMotors();
}
