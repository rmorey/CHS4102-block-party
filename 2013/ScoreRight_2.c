void ScoreRight(){
	int lightSTarget; //position  for right scoring
	int loc;
	int spower;
	int ioffset;

	wait1Msec(200);
	autoDrive(1,43.5);//proceed to white line
	autoDrive(3,3.0); //pushup against platform
	autoDrive(7,9.0); //pushup against platform

	//autoDrive(3,1.0); //backaway a little
	autoRotate(0.0-fGyroAngle);//rotate to face platform
	wait1Msec(50);

	angleAbortTest(20.0);

	putLiftUpAuto();

	//need to position servos
	hogCPU();
	servo[SV_TiltRight]=C_SVTILTRIGHTDOWN;
	servo[SV_TiltLeft]=C_SVTILTLEFTDOWN;
	releaseCPU();
	wait1Msec(200);
	servo[SV_Swivel]=C_SVSWIVELRIGHT;

	ClearTimer(T3); //nudge up to platform
	while(time1[T3]<3000){
		ClearTimer(T4);
		while(time1[T4]<50) goHolo(0,30,0);
		allDriveStop();
		wait1Msec(50);
	}

	angleAbortTest(20.0);
	reInitializeGyro();

	lightSTarget=130;
	loc=scanForMax(C_SVLIGHTSENSORIN-60,C_SVLIGHTSENSORMXOUT);
	direction*=-1;
	if(range<C_RANGESAFETY) StopAllTasks();
	setLight(loc+20);
	trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,500);


	ClearTimer(T3); //left-right positioning
	while(time1[T3]<2000){

		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,20);

		ioffset=ServoValue[SV_LightSensor]-lightSTarget;

		spower=abs(6*ioffset);
		if(spower>30) spower=30;

		if(ioffset>0) {	goGyro(-spower,0);	}
		if(ioffset<0) {	goGyro(spower,0);		}
	}
	allDriveStop();

	setLight(C_SVLIGHTSENSORIN);

	ClearTimer(T3); //nudge up again
	while(time1[T3]<1000){
		ClearTimer(T4);
		while(time1[T4]<50) goHolo(0,30,0);
		allDriveStop();
		wait1Msec(50);
	}

	angleAbortTest(20.0);
	reInitializeGyro();

	ClearTimer(T4); //bump up on platform
	while(time1[T4]<300) goHolo(0,70,0);
	allDriveStop();
	wait1Msec(50);

	ClearTimer(T4);//fix rotation for some time
	while(time1[T4]<1000){
		goMaintainGyro(0,0,0);
	}
	allDriveStop();


	lightSTarget=60;
	//find and lock on white line and adjust position
	loc=scanForMax(C_SVLIGHTSENSORIN-60,C_SVLIGHTSENSORMXOUT);
	direction*=-1;
	setLight(loc);

	if(range<C_RANGESAFETY) StopAllTasks();

	setLight(loc+20);
	wait1Msec(100);

	trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,1000);

	ClearTimer(T3); //position to score
	while(time1[T3]<3000){

		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,20);

		ioffset=ServoValue[SV_LightSensor]-lightSTarget;

		spower=abs(3*ioffset);
		if(spower>15) spower=15;

		if(ioffset>0) { goGyro(-spower,spower);	}
		if(ioffset<0) {	goGyro(spower,-spower);	}
	}
	allDriveStop();

	setLight(C_SVLIGHTSENSORIN);

	ClearTimer(T4);
	while(time1[T4]<500) goRotLS(30);
	allDriveStop();

	int signX=1,signY=1,power=15; //direction 7
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
	autoDrive(5,10.0);

	putLiftDownAuto();

	return;
}
