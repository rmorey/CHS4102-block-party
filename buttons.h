//buttons
#define SCORE_BTN joy2Btn(1)
#define RESET_BTN joy2Btn(2)
#define CONVEYOR_IN_BTN joy2Btn(3)
#define CONVEYOR_OUT_BTN joy2Btn(4)
#define LEFT_SLIDER_UP_BTN joy2Btn(5)
#define LEFT_SLIDER_DOWN_BTN joy2Btn(7)
#define RIGHT_SLIDER_UP_BTN joy2Btn(6)
#define RIGHT_SLIDER_DOWN_BTN joy2Btn(8)
#define FLAGGER_OUT_BTN joy1Btn(6)
#define FLAGGER_IN_BTN joy1Btn(5)
#define SPIN_FLAGGER_CW_BTN joy1Btn(8)
#define SPIN_FLAGGER_CCW_BTN joy1Btn(7)

//joysticks
#define DRIVE_JS_X joystick.joy1_x1
#define DRIVE_JS_Y joystick.joy1_y1
#define ROTATE_JS joystick.joy1_x2
#define BUCKET_JS joystick.joy2_y1

//tophats return a number in [0,7] when pressed, -1 when unpressed
#define BUCKET_DPAD joystick.joy2_TopHat
#define DRIVE_DPAD joystick.joy1_TopHat


void goForward (int power) {// these functions go in the specified direction at the specified power
	motor[M_drive_FL] = power;
	motor[M_drive_FR] = power;
	motor[M_drive_BR] = power;
	motor[M_drive_BL] = power;
}
void goBack (int power) {
	motor[M_drive_FL] = -power;
	motor[M_drive_FR] = -power;
	motor[M_drive_BR] = -power;
	motor[M_drive_BL] = -power;
}
void goLeft (int power) {
	motor[M_drive_FL] = -power;
	motor[M_drive_FR] = power;
	motor[M_drive_BR] = -power;
	motor[M_drive_BL] = power;
}
void goRight (int power) {
	motor[M_drive_FL] = power;
	motor[M_drive_FR] = -power;
	motor[M_drive_BR] = power;
	motor[M_drive_BL] = -power;
}
void goForwardLeft (int power) {
	motor[M_drive_FL] = 0;
	motor[M_drive_FR] = power;
	motor[M_drive_BR] = 0;
	motor[M_drive_BL] = power;
}
void goForwardRight (int power) {
	motor[M_drive_FL] = power;
	motor[M_drive_FR] = 0;
	motor[M_drive_BR] = power;
	motor[M_drive_BL] = 0;
}
void goBackRight (int power) {
	motor[M_drive_FL] = 0;
	motor[M_drive_FR] = -power;
	motor[M_drive_BR] = 0;
	motor[M_drive_BL] = -power;
}
void goBackLeft (int power) {
	motor[M_drive_FL] = -power;
	motor[M_drive_FR] = 0;
	motor[M_drive_BR] = -power;
	motor[M_drive_BL] = 0;
}

void haltAllMotors () { //stops all motors
	motor[M_drive_FL] = 0;
	motor[M_drive_FR] = 0;
	motor[M_drive_BR] = 0;
	motor[M_drive_BL] = 0;
	motor[M_belt] = 0;
	motor[M_slider_R] = 0;
	motor[M_slider_L] = 0;
	motor[M_flag] = 0;
}

void raiseBucket() { // raises bucket to topmost position
	nMotorEncoder[M_slider_R] = 0;
	nMotorEncoder[M_slider_L] = 0;

	nMotorEncoderTarget[M_slider_R] = 100; // TODO: fix these values
	nMotorEncoderTarget[M_slider_L] = 100;

	motor[M_slider_R] = 50;
	motor[M_slider_L] = 50;

	while (nMotorRunState[M_slider_R] != runStateIdle || nMotorRunState[M_slider_L] != runStateIdle){
		// waits until motors have stopped
	}

}

void scoreBlocks() { //tilts scoring servo to score blocks
	servo[S_score_1] = 10; //TODO: fix this
	servo[S_score_2] = 10;
}

void resetBucket() { // returns scoring servos to horizontal, and brings bucket back to bottom
	servo[S_score_1] = 10; //TODO: fix
	servo[S_score_2] = 10;

	nMotorEncoderTarget[M_slider_L] = 0; // sets encoder targets to bottom
	nMotorEncoderTarget[M_slider_R] = 0;

	motor[M_slider_R] = -50; // starts motors moving, encoders will stop them.
	motor[M_slider_L] = -50;

	while (nMotorRunState[M_slider_R] != runStateIdle || nMotorRunState[M_slider_L] != runStateIdle){
		// waits until motors have stopped
	}

}

void rotateBotPower(int power){ // rotate bot givesfpofer, positive is ccw, negative is cw
	motor[M_drive_FR] = power;
	motor[M_drive_FL] = -power;
	motor[M_drive_BR] = power;
	motor[M_drive_BL] = -power;
}

void rotateBotAngle(int angle){ // rotate bot given angle,positive is ccw, negative is cw
	nMotorEncoder[M_drive_FR] = 0; // sets encoder values to zero
	nMotorEncoder[M_drive_FL] = 0;
	nMotorEncoder[M_drive_BR] = 0;
	nMotorEncoder[M_drive_BL] = 0;

	nMotorEncoderTarget[M_drive_FR] = angle;
	nMotorEncoderTarget[M_drive_FL] = -angle;
	nMotorEncoderTarget[M_drive_BR] = angle;
	nMotorEncoderTarget[M_drive_BL] = -angle;

	int power = 50 * abs(angle);
	rotateBotPower(power); // starts rotating, encoders will stop them

	while (nMotorRunState[M_drive_FR] != runStateIdle || nMotorRunState[M_drive_BL] != runStateIdle){
		// waits until motors have stopped
	}

}
