int LiftEncoder(){
	return nMotorEncoder[M_Lift];
}


void displayLiftEncoder(){
  	nxtDisplayBigTextLine(1, "LIFT ENC");
  	nxtDisplayBigTextLine(3, "%d",nMotorEncoder[M_Lift]);
  	nxtDisplayBigTextLine(5, "%d",LiftEncoder());
}


int LiftTargetUpdate(){ //maintains lift movement to target
	int liftpower;
	if(!GB_liftTargetSet) {
		GI_liftTarget=LiftEncoder();
		motor[M_Lift]=0;
		return -1; //target not set
	}
	int currentLiftEncoder = LiftEncoder();
	liftpower=100;
	if(abs(currentLiftEncoder-GI_liftTarget)<50){
		motor[M_Lift]=0;
		GB_liftTargetSet=false;
		return 0;		//target reached
	}

	if(abs(currentLiftEncoder-GI_liftTarget)<800){
		liftpower=50;
	}
	if(abs(currentLiftEncoder-GI_liftTarget)<400){
		if(GI_liftTarget<6000) liftpower=30;
	}

	if(currentLiftEncoder < GI_liftTarget) {
		motor[M_Lift]=liftpower;  //target above
		return 1;
	}
	if(currentLiftEncoder > GI_liftTarget) {
		motor[M_Lift]=-liftpower; //target below
		return 1;
	}
	return 0;
}

void SetLiftTarget(int liftTarget){
	if(liftTarget<0) liftTarget=0;
	if(liftTarget>C_LIFTMAXENCODER) liftTarget=C_LIFTMAXENCODER;
	GI_liftTarget=liftTarget;
	GB_liftTargetSet=true;
}

void LiftDeltaInch(float dist){
	int currentLiftEncoder = LiftEncoder();
	int liftTarget = currentLiftEncoder+(int)floor(C_LIFTGAIN*dist);
	SetLiftTarget(liftTarget);
}
