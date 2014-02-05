#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Motor,  mtr_S1_C1_1,     M_DriveFL,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     M_DriveBR,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     M_DriveFR,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     M_DriveBL,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     M_Lift,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     M_Unused,      tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    SV_TiltLeft,          tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    SV_TiltRight,         tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    SV_Swivel,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    SV_LightSensor,       tServoStandard)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

bool  GB_liftTargetSet; //to indicate automatic target for lift
int   GI_liftTarget;    //target value
float GF_SwivelTarget; //for manual control of swivel
//float GF_TiltTarget;   //for manual control of tilt

#include "JoystickDriver.c"
#include "getJoyValues.c"
#include "constants.c"
#include "Driving.c"
#include "ServoFns.c"
#include "LiftFns.c"

void globalAbort(){
	motor[M_DriveFL]=0;
	motor[M_DriveBL]=0;
	motor[M_DriveFR]=0;
	motor[M_DriveBR]=0;
	motor[M_Lift]=0;

	GB_liftTargetSet=false;
}

void initializeRobot(){

  servoChangeRate[SV_TiltLeft]        = 10;
  servoChangeRate[SV_TiltRight]       = 10;
  servoChangeRate[SV_Swivel]          = 10;
  servoChangeRate[SV_LightSensor]      = 2;

  nMotorEncoder[M_Lift]    = 0;

  servo[SV_Swivel]=C_SVSWIVELCENTER;
  servo[SV_TiltRight]=C_SVTILTRIGHTUP;
  servo[SV_TiltLeft]=C_SVTILTLEFTUP;
  servo[SV_LightSensor]=C_SVLIGHTSENSORIN;

  GB_liftTargetSet=false;
}


task main()
{
	int vliftencoder;

	initializeRobot();
	waitForStart();
	ClearTimer(T1);
  ClearTimer(T2);

  while (true) {

  	servo[SV_LightSensor]=C_SVLIGHTSENSORIN;

  	//displayLiftEncoder();
    //displayTiltServo();
    //displaySwivelServo();

  	getJoyValues();


		//armjy1 controls lift or default to auto target
   	if(abs(armjy1)>0) {
   		vliftencoder=LiftEncoder();
   		GB_liftTargetSet=false;
   		if(
   			(armButton9==true) ||
   			(vliftencoder>C_LIFTMINENCODER && armjy1<0) ||
   			(vliftencoder<C_LIFTMAXENCODER && armjy1>0)
   			){
   			motor[M_Lift]= armjy1;
   			}
   			else{
   				motor[M_Lift]=0;
   			}
   	}
   	else {
   		LiftTargetUpdate();
   	}

   	////////////////////////////
   	//armjx1 controls swivel
   	if(abs(armjx2)>0 && getTiltAngleR()>0.9*C_SVSWIVELMAXANGLE) {
   		GF_SwivelTarget+=0.005*armjx2;
   		SetSwivel(GF_SwivelTarget);
   	}
   	else {
   		GF_SwivelTarget=getSwivelAngle();
   	}
   	////////////////////////////
   	//armjy1 controls tilt
   	/*
   	if(abs(armjy2)>0) {
   		GF_TiltTarget-=0.005*armjy2;
   		SetTilt(GF_TiltTarget);
   	}
   	else {
   		GF_TiltTarget=getTiltAngleL();
   	}
   	*/
		///////////////////////////////////
  	//arm buttons for lift presets
   	if(armTopHat==0) { GB_liftTargetSet=true; GI_liftTarget=C_LIFTTOPTARGET; }
   	if(armTopHat==2) { GB_liftTargetSet=true; GI_liftTarget=C_LIFTMIDTARGET; }
   	if(armTopHat==6) { GB_liftTargetSet=true; GI_liftTarget=C_LIFTLOWTARGET; }
   	if(armTopHat==4) { GB_liftTargetSet=true; GI_liftTarget=C_LIFTDISPENSE; }
   	if(armButton5 || armButton7)  GB_liftTargetSet=false;
   	if(armButton9)  {
   		GB_liftTargetSet=false;
   		nMotorEncoder[M_Lift]    = 0;
   	}

   	if(armButton4) {
   		servo[SV_Swivel]=C_SVSWIVELCENTER;
   		DriveWait1Msec(150);
   		SetTilt(0);
   	}
   	if(armButton2) {
   		SetTilt(C_SVTILTMAXANGLE);
   	}

   	//emergency
   	if(driveButton9) globalAbort();//!!!!!!!!!!!!!!
   	///////////////////////////////////////////////////////

		int thpower=30;
		switch(driveTopHat)
		{
		case -1:
			DriveUpdate();
			break;
		case 0:
			motor[M_DriveFL] = thpower;
			motor[M_DriveBR] = thpower;
			motor[M_DriveFR] = -thpower;
			motor[M_DriveBL] = -thpower;
			break;
		case 6:
			motor[M_DriveFL] = -thpower;
			motor[M_DriveBR] = -thpower;
			motor[M_DriveFR] = -thpower;
			motor[M_DriveBL] = -thpower;
			break;
		case 2:
			motor[M_DriveFL] = thpower;
			motor[M_DriveBR] = thpower;
			motor[M_DriveFR] = thpower;
			motor[M_DriveBL] = thpower;
			break;
		case 4:
			motor[M_DriveFL] = -thpower;
			motor[M_DriveBR] = -thpower;
			motor[M_DriveFR] = thpower;
			motor[M_DriveBL] = thpower;
			break;
		}

  }
}