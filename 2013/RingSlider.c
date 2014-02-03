
int RingSliderNudge(float dist){
	int retv;
	int mPower,duration;
	if(dist>0) mPower=50;
	else mPower=-50;

	duration=abs(floor(100*dist));
	motor[Mtr_RingSlider]=mPower;
	retv=DriveWaitingAbort(duration);
	motor[Mtr_RingSlider]=0;
	return retv;
}


