#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S3,     SONAR,          sensorSONAR)
#pragma config(Sensor, S4,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     M_DRIVE_BL,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     M_DRIVE_FL,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     M_FLAG,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_LIFT_L,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_1,     M_DRIVE_FR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     M_DRIVE_BR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     M_LIFT_R,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     M_BELT,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C3_1,    SV_BUCKET_R,          tServoStandard)
#pragma config(Servo,  srvo_S2_C3_2,    SV_HOOK,              tServoStandard)
#pragma config(Servo,  srvo_S2_C3_3,    SV_BUCKET_L,          tServoStandard)
#pragma config(Servo,  srvo_S2_C3_4,    SV_LID,               tServoStandard)
#pragma config(Servo,  srvo_S2_C3_5,    SV_AUTO,              tServoStandard)
#pragma config(Servo,  srvo_S2_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define T_sensor T2

int SonarSmooth;

task SensorUpdate() {
	int last_sonar = SensorValue[SONAR];
	int this_sonar;

	while(true) {
		while (time1[T_sensor] < 20) // better than wait1Msec(20)
			wait1Msec(1);
		time1[T_sensor] = 0;

		//gyro update

		//sonar update + smoothing
		this_sonar = SensorValue[SONAR];
		//ignore jumps of greater than 10 unless jumping from the error value of 255
		if (abs(this_sonar - last_sonar) < 10 || last_sonar == 255)
			SonarSmooth = this_sonar;
		last_sonar = this_sonar;

	}
}

task main() {
	StartTask(SensorUpdate);
	while(true) {
		nxtDisplayTextLine(1,"IR:%d", SensorValue[IR]);
		nxtDisplayTextLine(2,"Raw Sonar   :%d", SensorValue[SONAR]);
		nxtDisplayTextLine(3,"Smooth Sonar:%d", SonarSmooth);
	}
}
