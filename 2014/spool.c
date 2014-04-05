#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S3,     SONAR,          sensorSONAR)
#pragma config(Sensor, S4,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     M_DRIVE_BL,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     M_DRIVE_FL,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     M_FLAG,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_LIFT_L,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_1,     M_DRIVE_BR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     M_DRIVE_FR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     M_LIFT_R,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     M_BELT,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//lift and belt controls directly from the nxt

bool lift = true;

task main() {
	nxtDisplayBigTextLine(1,"Lifter");
	while(true) {
		if (lift) {
			;
			nxtDisplayTextLine(3,"R:%8d",nMotorEncoder[M_LIFT_R]);
			nxtDisplayTextLine(4,"L:%8d",nMotorEncoder[M_LIFT_L]);
		}
		while (nNxtButtonPressed == kRightButton) {
			if (lift) {
				motor[M_LIFT_R] = -100;
				motor[M_LIFT_L] = 100;
			}
			else
			motor[M_BELT] = 100;
		}
		while (nNxtButtonPressed == kLeftButton) {
			if (lift) {
				motor[M_LIFT_R] = 100;
				motor[M_LIFT_L] = -100;
			}
			else
				motor[M_BELT] = -100;
		}
		if (nNxtButtonPressed == kEnterButton) {
			PlaySoundFile("! Click.rso");
			while(nNxtButtonPressed != -1) {
				//wait for button release
			}
			lift = !lift;
			eraseDisplay();
			if (lift)
				nxtDisplayBigTextLine(1,"Lifter");
			else
				nxtDisplayBigTextLine(1,"Belt");
		}
		motor[M_LIFT_R] = 0;
		motor[M_LIFT_L] = 0;
		motor[M_BELT] = 0;

	}
}
