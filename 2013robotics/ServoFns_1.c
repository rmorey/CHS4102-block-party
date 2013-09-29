
void displayTiltServo(){
  	nxtDisplayBigTextLine(1, "SV tilt");
  	nxtDisplayBigTextLine(3, "%d",ServoValue[SV_TiltLeft]);
  	nxtDisplayBigTextLine(5, "%d",ServoValue[SV_TiltRight]);
}

void displaySwivelServo(){
  	nxtDisplayBigTextLine(1, "Swiv SV");
  	nxtDisplayBigTextLine(3, "%d",ServoValue[SV_Swivel]);
}


float getTiltAngleL(){ //0 is straight, right positive, use degrees
	int servoV;
	servoV=ServoValue[SV_TiltLeft];
	float servof;
	servof= (servoV-C_SVTILTLEFTUP)/(float)(C_SVTILTLEFTDOWN-C_SVTILTLEFTUP);
	return C_SVTILTMAXANGLE*servof;
}

float getTiltAngleR(){ //0 is straight, right positive, use degrees
	int servoV;
	servoV=ServoValue[SV_TiltRight];
	float servof;
	servof= (servoV-C_SVTILTRIGHTUP)/(float)(C_SVTILTRIGHTDOWN-C_SVTILTRIGHTUP);
	return C_SVTILTMAXANGLE*servof;
}


void SetTilt(float angle){ //0 is straight up, down positive, use degrees
	float anglefrac;
	anglefrac = angle/C_SVTILTMAXANGLE;
	if(anglefrac<0.0) anglefrac=0.0;
	if(anglefrac>1.0) anglefrac=1.0;

	int servoR,servoL;
	servoR=(int)floor(0.5+C_SVTILTRIGHTUP+anglefrac*(C_SVTILTRIGHTDOWN-C_SVTILTRIGHTUP));
	servoL=(int)floor(0.5+C_SVTILTLEFTUP+anglefrac*(C_SVTILTLEFTDOWN-C_SVTILTLEFTUP));

	servo[SV_TiltRight]=servoR;
	servo[SV_TiltLeft]=servoL;
}

float getSwivelAngle(){ //0 is straight, right positive, use degrees
	int servoV;
	servoV=ServoValue[SV_Swivel];
	float servof;
	servof= (servoV-C_SVSWIVELLEFT)/(float)(C_SVSWIVELRIGHT-C_SVSWIVELLEFT);
	return -C_SVSWIVELMAXANGLE + 2.0*C_SVSWIVELMAXANGLE*servof;
}


void SetSwivel(float angle){ //0 is straight, right positive, use degrees
	float anglefrac;
	if(angle>C_SVSWIVELMAXANGLE) angle=C_SVSWIVELMAXANGLE;
	if(angle<-C_SVSWIVELMAXANGLE) angle=-C_SVSWIVELMAXANGLE;

	anglefrac=(angle+C_SVSWIVELMAXANGLE)/(2.0*C_SVSWIVELMAXANGLE);

	int servoV;
	servoV=(int)floor(0.5+C_SVSWIVELLEFT+anglefrac*(C_SVSWIVELRIGHT-C_SVSWIVELLEFT));

	servo[SV_Swivel]=servoV;
}
