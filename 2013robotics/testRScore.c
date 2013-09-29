/////////////////////////////////////////////////////////////////////////////////
void testScoreRight(){

/*
	autoDrive(1,44.5);//proceed to white line
	autoDrive(7,6.0); //pushup against platform

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

  autoDrive(0,4.0);//move up to platform
  wait1Msec(100);
  */
	//go up on platform
  float fGyroTarget=fGyroAngle;
	autoDrive(0,2.0);
	wait1Msec(100);
	ClearTimer(T4);
	while(time1[T4]<1000){
		goMaintainGyro(0,0,fGyroTarget);
	}
	//autoDrive(0,4.0);

	//find and lock on white line
	int loc;
	loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT);
	if(range<5) StopAllTasks();
	//direction*=-1;
  setLight(loc-10);
  trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,2000);
  wait1Msec(100);
  ClearTimer(T3);
  int spower;
  int ioffset=20;
	while(time1[T3]<5000){

		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,20);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);

		ioffset=ServoValue[SV_LightSensor]-220;

		spower=abs(2*ioffset);
		if(spower>15) spower=15;

		if(ioffset>0) {
			goGyro(-spower,0);
		}
		if(ioffset<0) {
			goGyro(spower,spower);
		}
	}
  allDriveStop();

  //setLight(102);
  setLight(C_SVLIGHTSENSORIN);
  wait1Msec(200);
  return;

	//track line in to position scoring
  //or get in position and move in-- timer based
  int signX=1,signY=1,power=10;
  ClearTimer(T4);
  fGyroTarget=fGyroAngle;
  while(time1[T4]<1000){
  	goMaintainGyro(signX*power,signY*power,fGyroTarget);
  	//trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);
  }

  return;


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
