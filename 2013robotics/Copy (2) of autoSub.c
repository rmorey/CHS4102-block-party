int getInt(const string cmdA,const string cmdB, int min, int max){
	int value=min;
	bool doloop=true;
	hogCPU();
	while(doloop){
		ClearTimer(T4);
		nxtDisplayCenteredBigTextLine(1,"%s", cmdA);
		nxtDisplayCenteredBigTextLine(3,"%s", cmdB);
		nxtDisplayCenteredBigTextLine(5, "%d",value);

		while(time10[T4]<20){};
		if(nNxtButtonPressed == 1) value++;
		if(nNxtButtonPressed == 2) value--;
		if(value>max) value=max;
		if(value<min) value=min;
		if(nNxtButtonPressed == 3) doloop=false;
	}
	ClearTimer(T4);
	while(time10[T4]<100){
		nxtDisplayCenteredBigTextLine(1,"%s" ,cmdA);
		nxtDisplayCenteredBigTextLine(3,"%s" ,cmdB);
		nxtDisplayCenteredBigTextLine(5, "Got %d",value);
		releaseCPU();
	}
	return value;

}

//can be used to test whether orientation is OK
void angleAbortTest(){
	if(abs(fGyroAngle-expectedGyro)>20.0) StopAllTasks();
}

void goHolo(int xpow, int ypow,int rot) //try to keep rotation on target while driving
{
		motor[M_DriveFL] = xpow + ypow + rot; //xpow + rot; xpow + ypow + rot;
	  motor[M_DriveBR] = xpow + ypow - rot; //xpow - rot;
		motor[M_DriveFR] = xpow - ypow + rot;
	  motor[M_DriveBL] = xpow - ypow - rot;
}

void goGyro(int xpow, int ypow) //try to keep rotation constant while driving
{
	int rot;
	float fGyroTarget=fGyroAngle;

	rot=0;
		rot = -(int)floor(2.0*(fGyroAngle-fGyroTarget));
		if(rot>10) rot=10;
		if(rot<-10) rot=-10;

		motor[M_DriveFL] = xpow + ypow + rot;
	  motor[M_DriveBR] = xpow + ypow - rot;
		motor[M_DriveFR] = xpow - ypow + rot;
	  motor[M_DriveBL] = xpow - ypow - rot;
}

void goMaintainGyro(int xpow, int ypow,float fGyroTarget) //try to keep rotation constant while driving
{
	int rot;

	rot=0;
		rot = -(int)floor(2.0*(fGyroAngle-fGyroTarget));
		if(rot>10) rot=10;
		if(rot<-10) rot=-10;

		motor[M_DriveFL] = xpow + ypow + rot;
	  motor[M_DriveBR] = xpow + ypow - rot;
		motor[M_DriveFR] = xpow - ypow + rot;
	  motor[M_DriveBL] = xpow - ypow - rot;
}

void goRot(int rot) //apply rotation power
{
		motor[M_DriveFL] =  rot;
	  motor[M_DriveBR] = -rot;
		motor[M_DriveFR] =  rot;
	  motor[M_DriveBL] = -rot;
}


void allDriveStop(){
	motor[M_DriveFL] = 0;
	motor[M_DriveBR] = 0;
	motor[M_DriveFR] = 0;
	motor[M_DriveBL] = 0;
}

void autoDrive(int direct,float distance){
	//direct is 0,1,2,3,4,5,6,7 is like top hat: 0 forward, 1 45 to front right, etc.
	//distance is in inches
	//diagonal range is different from other directions
	int enctarget,enccount,enctogo,signX,signY,power,powerscale;
	int usedEnc;

	/*DATA
	nMotorEncoder[M_DriveFR]=0;
	//14 inches for a count of 1000 -- straight ahead (0 30), 25in for 2000 //12.2V
	//13 inches for 1000 -- diagonal  (-30 30), 22in for 2000  //12.2V
	//11 inches for 1000 -- diagonal  (-15 15), 20in for 2000 //12.2V
	//11.5 inches for 1000 -- diagonal  (-20 20), 20in for 2000 //12.2V
	while(abs(nMotorEncoder[M_DriveFR])<2000){
		go(-20,20)
	}
	allDriveStop();
	return;*/
	switch (direct%2){
		case 0: enctarget=(int)floor(0.5+distance*150.0); powerscale=2; break;
		case 1: enctarget=(int)floor(0.5+distance*110.0); powerscale=1; break; //DIAGONAL good for 24in
	}
	switch (direct){
		case 0: signX= 0; signY= 1; usedEnc=M_DriveBR; break;
		case 1: signX= 1; signY= 1; usedEnc=M_DriveBR; break;
		case 2: signX= 1; signY= 0; usedEnc=M_DriveBR; break;
		case 3: signX= 1; signY=-1; usedEnc=M_DriveBL; break;
		case 4: signX= 0; signY=-1; usedEnc=M_DriveBL; break;
		case 5: signX=-1; signY=-1; usedEnc=M_DriveBR; break;
		case 6: signX=-1; signY= 0; usedEnc=M_DriveBR; break;
		case 7: signX=-1; signY= 1; usedEnc=M_DriveBL; break;
	}

	nMotorEncoder[usedEnc]=0;

	int safetime=2*enctarget+200; //rough guess 10in = 2200  2.2 seconds


	ClearTimer(T4);
	while(time1[T4]<safetime){
		enccount=abs(nMotorEncoder[usedEnc]);
		enctogo=enctarget-enccount;
		if(enctogo<=0) {
			allDriveStop();
			return;
		}
		power=enccount<enctogo?enccount:enctogo;
		power=(30+power/200);
		if(power>50) power=50;
		power*=powerscale;
		//if(enctogo<200) power/=2;
		//if(enctogo<100) power/=2;
		goGyro(signX*power,signY*power);
	}
	allDriveStop();

	//if here then safetime exceeded
	StopAllTasks();

	return;
}

float autoRotate(float angle){
	//direct is 0,1,2,3,4,5,6,7 is like top hat: 0 forward, 1 45 to front right, etc.
	//distance is in inches
	//diagonal range is different from other directions
	//int enctarget,enccount,enctogo,signX,signY,power;
	//int usedEnc;
	int rotp;
	float fGyroTarget=fGyroAngle+angle;
	float angleerror=angle;
	expectedGyro+=angle;


	int safetime=(int)floor(abs(angle)*100); //about 1.5 second for 45

	ClearTimer(T4);
	while(abs(fGyroAngle-fGyroTarget)>0.5 && time1[T4]<safetime){
		rotp=angleerror>0?20:-20;
		goRot(rotp);
		//goGyro(0,0,fGyroTarget);
		angleerror=fGyroTarget-fGyroAngle;
		//nxtDisplayBigTextLine(1,"%f",angleerror);
	}
	allDriveStop();
	if(time1[T4]>=safetime) StopAllTasks();
	return fGyroTarget;
}

void IncRotate(float oldfGyroTarget,float angle){
	int rotp;
	float fGyroTarget=oldfGyroTarget+angle;
	float angleerror=angle;
	expectedGyro+=angle;

	int safetime=(int)floor(abs(angle)*100); //about 1.5 second for 45

	while(abs(fGyroAngle-fGyroTarget)>0.5 && time1[T4]<safetime){
		rotp=angleerror>0?20:-20;
		goRot(rotp);
		angleerror=fGyroTarget-fGyroAngle;
	}
	allDriveStop();
	if(time1[T4]>=safetime) StopAllTasks();

	return;
}




	//////////////////////////////////////
	//
float LightSensorToDistanceOut(int count){ //returns distance in inches
	int cntdiff = C_SVLIGHTSENSORIN-count;
	float degrees = (180.0*cntdiff)/255.0;
	float dist = sinDegrees(degrees)*6.5;
	return dist;
}

	//////////////////////////////////////
	//
float LightSensorToDistanceIn(int count){ //returns distance in inches
	int cntdiff = C_SVLIGHTSENSORIN-count;
	float degrees = (180.0*cntdiff)/255.0;
	float dist = cosDegrees(degrees)*6.5;
	return dist;
}

void putLiftUpAuto(){
	//put up lift to lower height
  hogCPU();
  motor[M_Lift]=100;
  int safetime=1200; //1.2 seconds
  ClearTimer(T4);
  while(nMotorEncoder[M_Lift]<C_LIFTLOWTARGET-400 && time1[T4]<safetime){};
  motor[M_Lift]=0;
  if(time1[T4]>=safetime) StopAllTasks();
  releaseCPU();
}

void scoreRingsAuto(){
	hogCPU();
  motor[M_Lift]=-10;
  int safetime=2500;
  ClearTimer(T4);
  while( nMotorEncoder[M_Lift]>C_LIFTLOWTARGET-1000 && time1[T4]<safetime){
  	//swivel??
  	if(time100[T4]%2 == 0) goRot(10);
  	if(time100[T4]%2 == 1) goRot(-10);
  };
  allDriveStop();
  motor[M_Lift]=0;
  if(time1[T4]>=safetime) StopAllTasks();
  releaseCPU();
}

void putLiftDownAuto(){
	//put lift down
	int safetime;
  hogCPU();
  motor[M_Lift]=-50;
  safetime=1000;
  ClearTimer(T4);
  while( nMotorEncoder[M_Lift]>800 && time1[T4]<safetime){};
  if(time1[T4]>=safetime) { motor[M_Lift]=0; StopAllTasks(); }
  motor[M_Lift]=-10;
  safetime=1000;
  ClearTimer(T4);
  while( nMotorEncoder[M_Lift]>100 && time1[T4]<safetime){};
  if(time1[T4]>=safetime) { motor[M_Lift]=0; StopAllTasks(); }
  motor[M_Lift]=0;
  releaseCPU();
}



void ScoreLeft(){
	int lightSTarget=220; //light sensor target on left

	autoDrive(7,44.5);//proceed to white line
	autoDrive(5,3.0); //pushup against platform
	autoDrive(1,9.0); //pushup against platform

	autoDrive(5,1.0); //backaway a little
	autoRotate(45.0); //rotate to face platform

	angleAbortTest();

  putLiftUpAuto();

  //need to position servos
  hogCPU();
  servo[SV_TiltRight]=C_SVTILTRIGHTDOWN;
  servo[SV_TiltLeft]=C_SVTILTLEFTDOWN;
  releaseCPU();
  wait1Msec(100);
  servo[SV_Swivel]=C_SVSWIVELLEFT;

  angleAbortTest();

  float fGyroTarget=fGyroAngle;
	autoDrive(0,3.0); //go up to platform
	wait1Msec(100);
	//go up on platform
	autoDrive(0,2.0);
	wait1Msec(100);

	ClearTimer(T4);//fix rotation for some time
	while(time1[T4]<1000){
		goMaintainGyro(0,0,fGyroTarget);
	}

	//find and lock on white line and adjust position
	int loc;
	loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT);
	if(range<C_RANGESAFETY) StopAllTasks();
	//direction*=-1;
  setLight(loc-10);
  trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,2000);
  wait1Msec(100);
  ClearTimer(T3);
  int spower;
  int ioffset;
	while(time1[T3]<5000){

		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,20);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);

		ioffset=ServoValue[SV_LightSensor]-lightSTarget;

		spower=abs(3*ioffset);
		if(spower>15) spower=15;

		if(ioffset>0) {
			goGyro(-spower,0);

		}
		if(ioffset<0) {
			goGyro(spower,spower);
		}
	}
  allDriveStop();
  setLight(C_SVLIGHTSENSORIN);


	//track line in to position scoring
  int signX=-1,signY=1,power=10; //direction 7
  ClearTimer(T4);
  fGyroTarget=fGyroAngle;
  while(time1[T4]<1000){
  	goMaintainGyro(signX*power,signY*power,fGyroTarget);
  	if(time1[T4]>100) power=5;
  	//trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);
  }

  //SAFETY CHECK FOR LINE
  loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT);
  if(abs(loc-lightSTarget)>25) StopAllTasks();
  setLight(C_SVLIGHTSENSORIN);

	//score rings
  scoreRingsAuto();

	//back out and reset
  autoDrive(3,10.0);

  //need to reposition servos
  servo[SV_Swivel]=C_SVSWIVELCENTER;
  wait1Msec(100);
  hogCPU();
  servo[SV_TiltRight]=C_SVTILTRIGHTUP;
  servo[SV_TiltLeft]=C_SVTILTLEFTUP;
  releaseCPU();

 //put lift down
  putLiftDownAuto();
}

////////////////////////////////////////////////////////////////
void ScoreMiddle(){
	int lightSTarget=70;

	//proceed to white line
	autoDrive(1,26.0);
	autoDrive(3,3.0); //pushup against platform
	autoDrive(7,9.0); //pushup against platform
	autoDrive(3,1.0); //backaway a little
	autoRotate(-45.0);//rotate to face platform
	wait1Msec(200);

	angleAbortTest();

  putLiftUpAuto();

  //need to position servos
  hogCPU();
  servo[SV_TiltRight]=C_SVTILTRIGHTDOWN;
  servo[SV_TiltLeft]=C_SVTILTLEFTDOWN;
  releaseCPU();
  wait1Msec(100);
  servo[SV_Swivel]=C_SVSWIVELRIGHT;

  angleAbortTest();

  float fGyroTarget=fGyroAngle;

  autoDrive(0,4.0);
  wait1Msec(200);
	autoDrive(0,9); //go up on platform
	wait1Msec(200);

	ClearTimer(T4);//fix rotation
	while(time1[T4]<1500){
		goMaintainGyro(0,0,fGyroTarget);
	}

	//find and lock on white line
	int loc;
	loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT);
	if(range<C_RANGESAFETY) StopAllTasks();
	direction*=-1;
  setLight(loc+10);
  trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,2000);
  wait1Msec(100);
  ClearTimer(T3);
  int spower;
  int ioffset;
	while(time1[T3]<5000){

		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,20);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);

		ioffset=ServoValue[SV_LightSensor]-lightSTarget;

		spower=abs(3*ioffset);
		if(spower>15) spower=15;

		if(ioffset>0) {
			goGyro(-spower,spower);

		}
		if(ioffset<0) {
			goGyro(spower,0);
		}
	}
  allDriveStop();
  setLight(C_SVLIGHTSENSORIN);

	//track line in to position scoring
  //or get in position and move in-- timer based
  int signX=1,signY=1,power=10;
  ClearTimer(T4);
  fGyroTarget=fGyroAngle;
  while(time1[T4]<1000){
  	goMaintainGyro(signX*power,signY*power,fGyroTarget);
  	if(time1[T4]>100) power=5;
  	//trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);
  }

  angleAbortTest();

  //SAFETY CHECK FOR LINE
  loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT);
  if(abs(loc-lightSTarget)>25) StopAllTasks();
  setLight(C_SVLIGHTSENSORIN);

	//score rings
	scoreRingsAuto();

	//back out and reset
  autoDrive(3,10.0);

  //need to reposition servos
  servo[SV_Swivel]=C_SVSWIVELCENTER;
  wait1Msec(100);
  hogCPU();
  servo[SV_TiltRight]=C_SVTILTRIGHTUP;
  servo[SV_TiltLeft]=C_SVTILTLEFTUP;
  releaseCPU();

 //put lift down
  putLiftDownAuto();
}

/////////////////////////////////////////////////////////////////////////////////
void ScoreRight(){
	int lightSTarget=70; //position  for right scoring

	autoDrive(1,36.0);//proceed to white line
	autoDrive(3,3.0); //pushup against platform
	autoDrive(7,9.0); //pushup against platform

	autoDrive(3,1.0); //backaway a little
	autoRotate(-45.0);//rotate to face platform

	angleAbortTest();

	//put up lift to lower height
  putLiftUpAuto();

  //need to position servos
  hogCPU();
  servo[SV_TiltRight]=C_SVTILTRIGHTDOWN;
  servo[SV_TiltLeft]=C_SVTILTLEFTDOWN;
  releaseCPU();
  wait1Msec(100);
  servo[SV_Swivel]=C_SVSWIVELRIGHT;

  angleAbortTest();

  float fGyroTarget=fGyroAngle;
  autoDrive(0,4.0);//move up to platform
  wait1Msec(100);
	//go up on platform
	autoDrive(0,2.0);
	wait1Msec(100);
	//autoDrive(0,4.0);

	ClearTimer(T4);//fix rotation
	while(time1[T4]<1000){
		goMaintainGyro(0,0,fGyroTarget);
	}

	//find and lock on white line
	int loc;
	loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT);
	if(range<C_RANGESAFETY) StopAllTasks();
	direction*=-1;
  setLight(loc+10);
  trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,2000);
  wait1Msec(100);
  ClearTimer(T3);
  int spower;
  int ioffset;
	while(time1[T3]<5000){

		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,20);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);

		ioffset=ServoValue[SV_LightSensor]-lightSTarget;

		spower=abs(3*ioffset);
		if(spower>15) spower=15;

		if(ioffset>0) {
			goGyro(-spower,spower);

		}
		if(ioffset<0) {
			goGyro(spower,0);
		}
	}
  allDriveStop();
  setLight(C_SVLIGHTSENSORIN);

	//track line in to position scoring
  //or get in position and move in-- timer based
  int signX=1,signY=1,power=10;
  ClearTimer(T4);
  fGyroTarget=fGyroAngle;
  while(time1[T4]<500){
  	goMaintainGyro(signX*power,signY*power,fGyroTarget);
  	if(time1[T4]>100) power=5;
  	//trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);
  }

  angleAbortTest();

    //SAFETY CHECK FOR LINE
  loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT);
  if(abs(loc-lightSTarget)>25) StopAllTasks();
  setLight(C_SVLIGHTSENSORIN);

	//score rings
  scoreRingsAuto();

	//back out and reset
  autoDrive(3,10.0);

  //need to reposition servos
  servo[SV_Swivel]=C_SVSWIVELCENTER;
  wait1Msec(100);
  hogCPU();
  servo[SV_TiltRight]=C_SVTILTRIGHTUP;
  servo[SV_TiltLeft]=C_SVTILTLEFTUP;
  releaseCPU();

  putLiftDownAuto();
}
