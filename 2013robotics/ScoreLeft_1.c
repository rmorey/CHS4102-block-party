void ScoreLeft(){
	int lightSTarget; //light sensor target on left
	int loc;

	autoDrive(5,2.0); //a little adjustment backleft
	autoRotate(-2.0);

	autoDrive(7,45.0);//proceed to white line
	autoDrive(5,3.0); //backup  a little (in case on platform)
	autoDrive(1,9.5); //pushup against platform

	autoDrive(5,0.5); //backaway a little
	wait1Msec(200);
	autoRotate(90.0-fGyroAngle); //rotate to face platform
	wait1Msec(50);

	angleAbortTest(20.0); //test the angle is OK

	/////////////////////
	putLiftUpAuto();
	//need to position servos left
	hogCPU();
	servo[SV_TiltRight]=C_SVTILTRIGHTDOWN;
	servo[SV_TiltLeft]=C_SVTILTLEFTDOWN;
	releaseCPU();
	wait1Msec(200);
	servo[SV_Swivel]=C_SVSWIVELRIGHT;
	/////////////////////////

	nudgeUpFront(3000);
	angleAbortTest(20.0);
	reInitializeGyro(); //reset servo angle


	loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT+60);
	if(range<C_RANGESAFETY) StopAllTasks();
	setLight(loc-20);
	wait1Msec(200);
	trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,500);

	//lightSTarget=180;
	lightSTarget=110;
	leftRightAdjust(2000,lightSTarget);
	setLight(C_SVLIGHTSENSORIN);

	nudgeUpFront(1200);
	wait1Msec(100);
	angleAbortTest(20.0);
	reInitializeGyro();

	bumpUpFrontA();

	ClearTimer(T4);//fix rotation for some time
	while(time1[T4]<1000){
		goMaintainGyro(0,0,0);
	}
	allDriveStop();


	//find and lock on white line and adjust position
	loc=scanForMax(C_SVLIGHTSENSORIN,C_SVLIGHTSENSORMXOUT+60);
	setLight(loc-20);
	wait1Msec(200);

	if(range<C_RANGESAFETY) StopAllTasks();
	//direction*=-1;

	//move right to correct line
	trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,500);
	lightSTarget=200;
	leftRightAdjust(2000,lightSTarget);

	//need to position servos left
	hogCPU();
	servo[SV_TiltRight]=C_SVTILTRIGHTDOWN;
	servo[SV_TiltLeft]=C_SVTILTLEFTDOWN;
	releaseCPU();
	servo[SV_Swivel]=C_SVSWIVELLEFT;

	//move diagonally to score
	lightSTarget=220;
	int ioffset,spower;
	ClearTimer(T3);
	while(time1[T3]<1500){
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,40);
		allDriveStop();
		trackEdge(C_SVLIGHTSENSORMXOUT,C_SVLIGHTSENSORIN,40);

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
