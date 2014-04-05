#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     MPLEX,          sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     M_DRIVE_BL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     M_DRIVE_FL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     M_BELT,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_LIFT_L,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S2_C1_1,     M_DRIVE_BR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     M_DRIVE_FR,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C3_1,     M_LIFT_R,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C3_2,     M_FLAG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C2_1,    SV_BUCKET_L,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C2_2,    SV_HOOK,              tServoStandard)
#pragma config(Servo,  srvo_S2_C2_3,    SV_BUCKET_R,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C2_4,    SV_AUTO,              tServoStandard)
#pragma config(Servo,  srvo_S2_C2_5,    SV_LID,               tServoStandard)
#pragma config(Servo,  srvo_S2_C2_6,    servo6,               tServoNone)


task main()
{



}
