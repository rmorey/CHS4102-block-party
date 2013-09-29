bool B_adjustingRot; //for use in main
bool B_adjustingPull;//for use in main

double len2 = 8.675;//longer branch of piston
double len1 = 6.5;  //shorter branch of piston

double f_pullTarget;
double f_rotTarget;

double ServoToPosition(int sv) //distance out converted to command for servo
{
	double len,theta,h,diff;
	diff=(C_PULLSLIDERIN-C_PULLSLIDEROUT);
	theta=3.14159*(sv-C_PULLSLIDEROUT)/diff;
//	theta=/((float)(C_PULLSLIDERIN-C_PULLSLIDEROUT));
	h=len1*sin(theta);
	len=sqrt(len2*len2-h*h)+len1*cos(theta);
	return len-(len2-len1);
}

int PositionToServo(double position) //distance out converted to command for servo
{
	double len,theta;
	len=len2-len1+position;
	if(len<len2-len1) len=len2-len1;
	if(len>len2+len1) len=len2+len1;
	if(len>12) len=12; //due to slider limit

	theta=C_PULLSLIDEROUT+(C_PULLSLIDERIN-C_PULLSLIDEROUT)*acos((len*len+len1*len1-len2*len2)/(2.0*len*len1))/3.14159;

	return (int)floor(0.5+theta);
}


void putPullSliderAllOut(){ //slider all the way out
	servo[PullSliderServo]=C_PULLSLIDEROUT;
}
void putPullSliderAllIn(){  //slider all the way in
	servo[PullSliderServo]=C_PULLSLIDERIN;
}

void adjustPullSlider(double amount){  //adjust position of slider by amount (inches)
	int sv=ServoValue(PullSliderServo);
	float currentPos=ServoToPosition(sv);
	int targetsv = PositionToServo(currentPos+amount);
	servo[PullSliderServo]=targetsv;
}

void putPullSliderTo(double position){  //set slider position
	int targetsv = PositionToServo(position);
	servo[PullSliderServo]=targetsv;
}

double getPullSliderPosition(){
	int sv=ServoValue(PullSliderServo);
	return ServoToPosition(sv);
}

///////////////////////////////////////////////////////////////////////

double RotToAngle(int sv){
	return 180.0*sv/255.0;
}

int AngleToRot(float angle){
	if(angle<0) angle=0;
	if(angle>180.0) angle=180.0;
	return (int)floor(0.5+255.0*angle/180.0);
}

void adjustRotServo(double amount){  //adjust position of slider by degrees
	int sv=ServoValue(RotServo);
	float currentAngle=RotToAngle(sv);
	int targetsv = AngleToRot(currentAngle+amount);
	servo[RotServo]=targetsv;
}

double getRotAngle(){
	int sv=ServoValue(RotServo);
	return RotToAngle(sv);
}

void putRotServoTo(double angle){  //set rot angle
	int targetsv = AngleToRot(angle);
	servo[RotServo]=targetsv;
}
