#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S3,     SONAR,          sensorSONAR)
#pragma config(Sensor, S4,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     M_DRIVE_BL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     M_DRIVE_FL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     M_BELT,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_LIFT_L,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     M_DRIVE_BR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     M_DRIVE_FR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C3_1,     M_LIFT_R,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C3_2,     M_FLAG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    SV_SCORE,             tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_2,    SV_HOOK,              tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_3,    SV_LID,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    SV_AUTO,              tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)

task main() {
	while (true) {
		nxtDisplayTextLine(1,"M_LIFT_R:%5d",nMotorEncoder[M_LIFT_R]);
		nxtDisplayTextLine(2,"M_LIFT_L:%5d",nMotorEncoder[M_LIFT_L]);
		nxtDisplayTextLine(3,"M_DRIVE_FR:%5d",nMotorEncoder[M_DRIVE_FR]);
		nxtDisplayTextLine(4,"M_DRIVE_FL:%5d",nMotorEncoder[M_DRIVE_FL]);
		nxtDisplayTextLine(5,"M_DRIVE_BR:%5d",nMotorEncoder[M_DRIVE_BR]);
		nxtDisplayTextLine(6,"M_DRIVE_BL:%5d",nMotorEncoder[M_DRIVE_BL]);
	}
}
