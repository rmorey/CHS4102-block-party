bool B_liftTargetSet;
float f_targetHeight;


void PL_goUpto_Enctarget(int target){
	//if not beyond target set motors
	//otherwise stop and unset target flag
	int current=nMotorEncoder[Mtr_ParaLift];
	int error = target-current;
	//int expecttime = abs(error)*10;
	if(error>0){
		motor[Mtr_ParaLift]=100;
	}
	else{
		motor[Mtr_ParaLift]=0;
		B_liftTargetSet=false;
	}
}

void PL_goDownto_Enctarget(int target){
	//if not beyond target set motors
	//otherwise stop and unset target flag
	int current=nMotorEncoder[Mtr_ParaLift];
	int error = target-current;
	//int expecttime = abs(error)*10;
	if(error>0){
		motor[Mtr_ParaLift]=-100;
	}
	else{
		motor[Mtr_ParaLift]=0;
		B_liftTargetSet=false;
	}
}

int PL_height_to_Enc(float height){
	//convert height in inches to encoder target
	float f_enc;
	if(height<0) return 0;
	if(height>36.0) return C_MAXPLENCODER;
	f_enc=C_MAXPLENCODER*(height/36.0);
	return (int)floor(f_enc);
}
float PL_Enc_to_height(int enc){
	//convert height in inches to encoder target
	float height;
	height=36.0*enc/C_MAXPLENCODER;
	return height;
}

int PL_set_height_target(float height){
	//convert height in inches to encoder target
	float currentH;
	int enc_target;
	currentH=PL_Enc_to_height(nMotorEncoder[Mtr_ParaLift]);
	if(abs(currentH-height)<0.5){
		B_liftTargetSet=false;
		return 0;
	}
	enc_target=PL_height_to_Enc(height);
	if(height>currentH){
		PL_goUpto_Enctarget(enc_target);
	}
	if(height<currentH){
		PL_goDownto_Enctarget(enc_target);
	}
	return 1;
}
