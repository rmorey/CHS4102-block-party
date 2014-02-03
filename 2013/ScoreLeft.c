void ScoreLeft(){
	int lightSTarget=220; //light sensor target on left
	int loc;

	autoDrive(5,3.0); //a little adjustment backleft
	autoRotate(-2.0);

	autoDrive(7,45.0);//proceed to white line
	autoDrive(5,3.0); //backup  a little (in case on platform)
	autoDrive(1,9.5); //pushup against platform

	autoDrive(5,0.5); //backaway a little
	wait1Msec(200);
	autoRotate(90.0-fGyroAngle); //rotate to face platform
	wait1Msec(50);

	angleAbortTest(20.0); //test the angle is OK

	putLiftUpAuto();

	//need to position servos
	hogCPU();
	servo[SV_TiltRight]=C_SVTILTRIGHTDOWN;
	servo[SV_TiltLeft]=C_SVTILTLEFTDOWN;
	releaseCPU();
	wait1Msec(200);
	servo[SV_Swivel]=C_SVSWIVELLEFT;

	ClearTimer(T3); //nudging up
	while(time1[T3]<3000){
		ClearTimer(T4);
		while(time1[T4]<50) goHolo(0,30,0);
		allDriveStop();
		wait1Msec(50);
	}

	angleAbortTest(20.0);
	reInitializeGyro(); //reset servo angle

	lightSTarget=180;
	loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT+60);
	if(range<C_RANGESAFETY) StopAllTasks();
	setLight(loc-20);
	wait1Msec(200);
	trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,500);

	int spower;
	int ioffset;

	ClearTimer(T3); //left-right adjustment
	while(time1[T3]<2000){
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,20);

		ioffset=ServoValue[SV_LightSensor]-lightSTarget;
		spower=abs(6*ioffset);
		if(spower>30) spower=30;

		if(ioffset>0) {	goGyro(-spower,0); }
		if(ioffset<0) {	goGyro(spower,0); }
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

	ClearTimer(T4);
	while(time1[T4]<300) goHolo(0,70,0); //bump up
	allDriveStop();
	wait1Msec(50);

	ClearTimer(T4);//fix rotation for some time
	while(time1[T4]<1000){
		goMaintainGyro(0,0,0);
	}
	allDriveStop();

	lightSTarget=220;
	//find and lock on white line and adjust position
	loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT+60);
	setLight(loc);

	if(range<C_RANGESAFETY) StopAllTasks();
	//direction*=-1;
	setLight(loc-20);
	wait1Msec(200);
	trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,1000);

	ClearTimer(T3);  //position to score
	while(time1[T3]<3000){
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,50);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,20);

		ioffset=ServoValue[SV_LightSensor]-lightSTarget;

		spower=abs(3*ioffset);
		if(spower>15) spower=15;

		if(ioffset>0) {	goGyro(-spower,-spower);	}
		if(ioffset<0) {	goGyro(spower,spower);	}
	}
	allDriveStop();
	setLight(C_SVLIGHTSENSORIN);

	ClearTimer(T4); //rotate in to score
	while(time1[T4]<500) goRotRS(-30);
	allDriveStop();

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
