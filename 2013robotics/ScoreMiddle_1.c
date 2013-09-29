


void ScoreMiddle(){
	int lightSTarget; //position  for right scoring
	int loc;
	int spower;
	int ioffset;

	autoDrive(5,3.0);
	autoRotate(-2.0);

	autoDrive(7,18.5);//proceed to white line
	autoDrive(1,9.5); //pushup against platform

	autoDrive(5,0.5); //backaway a little
	wait1Msec(200);
	autoRotate(87.0-fGyroAngle); //rotate to face platform
	wait1Msec(50);

	angleAbortTest(20.0);

	////////////OLD LIFT LOCATION
	//////////////////////////////////////////
	putLiftUpAuto();

	//need to position servos
	hogCPU();
	servo[SV_TiltRight]=C_SVTILTRIGHTDOWN;
	servo[SV_TiltLeft]=C_SVTILTLEFTDOWN;
	releaseCPU();
	wait1Msec(200);
	servo[SV_Swivel]=C_SVSWIVELLEFT;
	///////////////////////////////////

	autoDrive(0,0.5);
	nudgeUpFront(2000);
	angleAbortTest(20.0);
	reInitializeGyro();

	bumpUpFrontA();

	//find and lock on white line and adjust position
	loc=scanForMax(C_SVLIGHTSENSORIN-60,C_SVLIGHTSENSORMXOUT);
	direction*=-1;
	setLight(loc+20);
	wait1Msec(100);
	if(range<C_RANGESAFETY) StopAllTasks();
	trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,500);
	lightSTarget=80;
	leftRightAdjust(2000,lightSTarget);
	setLight(C_SVLIGHTSENSORIN);



	allDriveStop();

	autoDrive(0,3);

	ClearTimer(T4);//fix rotation for some time
	while(time1[T4]<500){
		goMaintainGyro(0,0,0);
	}
	allDriveStop();

	nudgeUpBack(2000);

	angleAbortTest(20.0);
	reInitializeGyro();

	loc=scanForMax(C_SVLIGHTSENSORIN-110,C_SVLIGHTSENSORMXOUT);
	//direction*=-1;
	setLight(loc+20);
	wait1Msec(100);
	if(range<C_RANGESAFETY) StopAllTasks();
	trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,500);
	lightSTarget=190;
	leftRightAdjust(2000,lightSTarget);
	setLight(C_SVLIGHTSENSORIN);

	nudgeUpBack(1000);

	//bump up back wheels
	ClearTimer(T4);
	while(time1[T4]<350) goHolo(0,85,0);
	allDriveStop();
	wait1Msec(50);

	ClearTimer(T4);//fix rotation for some time
	while(time1[T4]<1000){
		goMaintainGyro(0,0,0);
	}
	allDriveStop();

	loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT+60);
	if(range<C_RANGESAFETY) StopAllTasks();

	setLight(loc-20);
	trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,1000);
	wait1Msec(100);

	lightSTarget=208;
	ClearTimer(T3); //track line for scoring position
	while(time1[T3]<1000){

		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,20);

		ioffset=ServoValue[SV_LightSensor]-lightSTarget;

		spower=abs(3*ioffset);
		if(spower>10) spower=10;

		if(ioffset>0) {	goGyro(-spower,-spower);	}
		if(ioffset<0) {	goGyro(spower,spower);		}
	}
	allDriveStop();

	setLight(C_SVLIGHTSENSORIN);

	ClearTimer(T4);
	while(time1[T4]<700) goRotRS(-30);
	allDriveStop();


	//track line in to position scoring
	int signX=-1,signY=1,power=15; //direction 7
	ClearTimer(T4);
	float fGyroTarget=fGyroAngle;
	while(time1[T4]<1000){
		goMaintainGyro(signX*power,signY*power,fGyroTarget);
		if(time1[T4]>500) power=5;
	}
	allDriveStop();

	//score rings
	scoreRingsAuto();

	//back out and reset
	autoDrive(3,10.0);

	putLiftDownAuto();

	return;
}
