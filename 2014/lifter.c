#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C2_2,     M_LIFT_L,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S2_C3_1,     M_LIFT_R,    tmotorTetrix, openLoop, encoder)

//lifter controls directly from the nxt

task main()
{
	while(true) {
		while (nNxtButtonPressed == 1) {
			motor[M_LIFT_R] = 100;
			motor[M_LIFT_L] = 100;
		}
		while (nNxtButtonPressed == 2) {
			motor[M_LIFT_R] = -100;
			motor[M_LIFT_L] = -100;
		}
		motor[M_LIFT_R] = 0;
		motor[M_LIFT_L] = 0;
	}
}
