int tkthreshold;  //defines edge boundary
int range;      //range across edge
int direction;  //orientation relative to servo
int readings[45]; // to store readings
int rsigns[45];   // for classifying
int step,nstep;
float filteredsv;
float avgP,avgM;
float srvtarg;

void tdisplay(int arg=0){
	nxtDisplayBigTextLine(1, "%d", arg);
	nxtDisplayBigTextLine(3, "%d", ServoValue[SV_LightSensor]);
  nxtDisplayBigTextLine(5, "%d", SensorValue[S_lightsensor]);
}

void mwait(int t,int arg=0){
	ClearTimer(T1);
	while(time1[T1]<t){ tdisplay(arg); }
}


void scanRegion(int first,int last){
	//arguments are first and last servo position readings for scan
	//scans accross looking for transition in light sensor
	//sets up tkthreshold, range and direction parameters for edge tracking

	int rval,rmin,rmax; //light sensor readings, min and max
	int i;

	int cntM,cntP;
	int dsign;
	int servTarg;
	rmin=10000; rmax=-10000;

	if(first<C_SVLIGHTSENSORMXOUT) first = C_SVLIGHTSENSORMXOUT;
	if(last<C_SVLIGHTSENSORMXOUT) last = C_SVLIGHTSENSORMXOUT;
	if(first>C_SVLIGHTSENSORIN) first = C_SVLIGHTSENSORIN;
	if(last>C_SVLIGHTSENSORIN) last = C_SVLIGHTSENSORIN;

	dsign=last>first?1:-1;
	nstep=40;
	step=(last-first)/40;
	if(step==0) {
		step=dsign;
		nstep=abs(last-first);
	}
	servo[SV_LightSensor]=first;

	servTarg=first;
	for(i=0;i<nstep;i++){
		servo[SV_LightSensor]=servTarg;
		servTarg+=step;
		mwait(30);
		rval=SensorValue[S_lightsensor];
		if(rval>rmax) rmax=rval;
		if(rval<rmin) rmin=rval;
		readings[i]=rval;
	}
	tkthreshold=(rmin+rmax)/2;

	range=rmax-rmin;

	//get averages for better tkthreshold
	avgM=0;avgP=0;cntM=0;cntP=0;
	for(i=0;i<nstep;i++){
		rsigns[i]=readings[i]<tkthreshold?-1:1;
		if(rsigns[i]<0) {avgM+=readings[i]; cntM++; }
		else { avgP+=readings[i]; cntP++;}
	}
	avgM/=cntM;
	avgP/=cntP;
	tkthreshold=(int)floor(0.5+(avgM+avgP)/2);

	range=(int)(1+(int)floor(avgP-avgM));
}

void setLight(int loc){
	if(loc<C_SVLIGHTSENSORMXOUT) loc = C_SVLIGHTSENSORMXOUT;
	if(loc>C_SVLIGHTSENSORIN) loc = C_SVLIGHTSENSORIN;
	servo[SV_LightSensor]=loc;
	srvtarg=(float)loc;
	filteredsv=(float)loc;
}

int scanForEdge(int first,int last){
	//direction parameters for edge tracking
  //moves to first edge location
	int i;
	scanRegion(first,last);
	direction=(rsigns[nstep-1]-rsigns[0])/2;  //is it getting brighter or darker?

	i=0;
	while(direction*(tkthreshold-readings[i])>0) i++;

	if(last<first) direction*=-1; //correct direction for orientation --
	filteredsv=first+i*step;
//	gftmp=tkthreshold;
	return(first+i*step);
}

int scanForMax(int first,int last){
	//looks for transition
	//assumes line is brighter
	int i;
	int rmax,imax,rmin;
	scanRegion(first,last);
	rmax=-10000;
	rmin=10000;
	for(i=0;i<nstep;i++){
		if(readings[i]>rmax) { imax=i; rmax=readings[i]; }
		if(readings[i]<rmin) {         rmin=readings[i]; }
	}
	tkthreshold=(rmax+avgM)/2; //adjustment for thin line (average of peak can be depressed)
	direction = 1;
	range=rmax-rmin;
	return(first+imax*step);//returns location of max
}


int signSum=0;
//float sensorrate;
float EdgeError(){
	int rval,sign;
	float poserr;
//	int svval;

	float maxposerr=0.07;

		rval=SensorValue[S_lightsensor];
		poserr=(float)abs((tkthreshold-rval))/(float)range;
		poserr=poserr*(0.04+0.02*poserr);
		if(poserr>maxposerr) poserr=maxposerr;
		sign=tkthreshold>rval?direction:-direction;
		if(poserr<sign*signSum<0) signSum=0;
		return ((float)sign)*poserr;
}


void trackEdge(int min,int max,int time){
	int rval,sign;
	float poserr;
	int svval;
	int updtt=0;
	float maxposerr=0.07;


	//srvtarg=(float)servoValue[SV_LightSensor];
	ClearTimer(T4);
	while(time1[T4]<time){
		rval=SensorValue[S_lightsensor];
		poserr=(float)abs((tkthreshold-rval))/(float)range;
		poserr=poserr*(0.07+0.02*poserr);
		if(poserr>maxposerr) poserr=maxposerr;
		sign=tkthreshold>rval?direction:-direction;
		if(poserr<sign*signSum<0) signSum=0;
		else {
			signSum+=sign;
			//if(abs(signSum)>250)poserr*=1.5;
			//if(abs(signSum)>800)poserr*=3;
		}
		svval=ServoValue[SV_LightSensor];
		if(svval<min && sign==-1) sign=0;
		if(svval>max && sign==1) sign=0;
		srvtarg+=((float)sign)*poserr;

		servo[SV_LightSensor]=(int)floor(0.5+srvtarg);
		//mwait(40,(int)floor(poserr));
		//wait1Msec(1);
		//filteredsv=svval;
		if(time1[T4]>updtt){
			filteredsv=0.6*filteredsv+0.4*svval;
			updtt++;
		}
	}
}

/*
void mwait(int t,int arg=0){
	ClearTimer(T1);
	while(time1[T1]<t){ tdisplay(arg);};
}

int putMlight(int target,int eps=1){
	int current,power;
	current=nMotorEncoder[M_lightsensor];
	if(abs(current-target)<eps) return 0;
	nMotorEncoderTarget[M_lightsensor]=target-current;
	if(current<target) power=15;
	else power=-10;
	motor[M_lightsensor]=power;
	while(nMotorRunState[M_lightsensor] != runStateIdle){};
	motor[M_lightsensor]=0;
	return power;
}


void trackEdge(int min,int max,int time){
	int rval,power,sign;
	int encval;
	int updtt=0;
	ClearTimer(T2);
	while(time1[T2]<time){
		rval=SensorValue[S_lightsensor];
		power=abs(30*(tkthreshold-rval))/range;
		if(power>15) power=15;
		sign=tkthreshold>rval?direction:-direction;
		encval=nMotorEncoder[M_lightsensor];
		if(encval<min && sign==-1) sign=0;
		if(encval>max && sign==1) sign=0;
		motor[M_lightsensor]=sign*power;
		//tdisplay();
		if(time10[T2]>updtt){
			filteredEnc=0.9*filteredEnc+0.1*encval;
			updtt++;
		}
	}
	motor[M_lightsensor]=0;
}


void trackmax(int min,int max,int time){
	//tries to track local max
	int rval,rvalp,power,sign,rvalavg;
	ClearTimer(T2);
	rvalp=SensorValue[S_lightsensor];
	rvalavg=rvalp;
	sign=-1;
	while(time1[T2]<time){
		nMotorEncoderTarget[M_lightsensor]=sign*2;
		motor[M_lightsensor]=sign*10;
		while(nMotorRunState[M_lightsensor] != runStateIdle){};
		motor[M_lightsensor]=0;
		rval=SensorValue[S_lightsensor];
		if(rval<rvalp) sign*=-1;
		rvalavg=(rvalavg+rval)/2;
		rvalp=rval;
		if(nMotorEncoder[M_lightsensor]<min && sign==-1) sign=0;
		if(nMotorEncoder[M_lightsensor]>max && sign==1) sign=0;
	}
}

void scanRegion(int first,int last){
	//arguments are first and last encoder readings for scan
	//scans accross looking for transition in light sensor
	//sets up tkthreshold, range and direction parameters for edge tracking


	int rval,rmin,rmax; //light sensor readings, min and max
	int i;

	int cntM,cntP;
	int dsign;
	int res;
	rmin=10000; rmax=-10000;

	dsign=last>first?1:-1;
	nstep=20;
	step=(last-first)/20;
	if(step==0) {
		step=dsign;
		nstep=abs(last-first);
	}
	res=putMlight(first);
	while(res!=0) res=putMlight(first); //position light at first

	for(i=0;i<nstep;i++){
		nMotorEncoderTarget[M_lightsensor]=step;
		motor[M_lightsensor]=dsign*15+5;
		while(nMotorRunState[M_lightsensor] != runStateIdle){};
		motor[M_lightsensor]=0;
		mwait(25);
		rval=SensorValue[S_lightsensor];
		if(rval>rmax) rmax=rval;
		if(rval<rmin) rmin=rval;
		readings[i]=rval;
	}
	tkthreshold=(rmin+rmax)/2;
	range=rmax-rmin;

	//get averages for better tkthreshold
	avgM=0;avgP=0;cntM=0;cntP=0;
	for(i=0;i<nstep;i++){
		rsigns[i]=readings[i]<tkthreshold?-1:1;
		if(rsigns[i]<0) {avgM+=readings[i]; cntM++; }
		else { avgP+=readings[i]; cntP++;}
	}
	avgM/=cntM;
	avgP/=cntP;
	tkthreshold=(int)floor(0.5+(avgM+avgP)/2);
	range=(int)(1+(int)floor(avgP-avgM));
}

int scanForEdge(int first,int last){
	//direction parameters for edge tracking
  //moves to first edge location
	int i;
	scanRegion(first,last);
	direction=(rsigns[nstep-1]-rsigns[0])/2;  //is it getting brighter or darker?
	if(last<first) direction*=-1;
	i=0;
	while(direction*(tkthreshold-readings[i])>0) i++;

	return(first+i*step);
}



int scanForLine(int first,int last){
	//looks for transition
	//assumes line is brighter
	int i;
	int rmax,imax;
	scanRegion(first,last);
	rmax=-10000;
	for(i=0;i<nstep;i++){
		if(readings[i]>rmax) { imax=i; rmax=readings[i]; }
	}
	tkthreshold=(rmax+avgM)/2; //adjustment for thin line (average of peak can be depressed)
	return(first+imax*step);//returns location of max
}

int fastscanForLine(int first,int last){
	//looks for transition
	//assumes line is brighter
	int i;
	int rmax,rmin,rval;
	int power,maxloc,loc;
	putMlight(first);
	if(last>first) power=10;
	else power=-10;
	nMotorEncoderTarget[M_lightsensor]=last-first;
	motor[M_lightsensor]=power;
	rmax=-10000;
	rmin=10000;
	while(nMotorRunState[M_lightsensor] != runStateIdle){
		loc=nMotorEncoder[M_lightsensor];
		rval=SensorValue[S_lightsensor];
		if(rval>rmax){rmax=rval; maxloc=loc;}
		if(rval<rmin){rmin=rval;}
	};
	motor[M_lightsensor]=0;
	tkthreshold=(rmax+rmin)/2;
	range=rmax-rmin;
	return(maxloc);//returns location of max
}
*/

void writeScanArray(){
	TFileHandle myFileHandle;          // create a file handle variable 'myFileHandle'
	TFileIOResult IOResult;            // create an IO result variable 'IOResult'
	string myFileName = "readA.txt";  // create and initialize a string variable 'myFileName'
	int myFileSize = 400;               // create and initialize an integer variable 'myFileSize'

	OpenWrite(myFileHandle, IOResult, myFileName, myFileSize);  // open for write: "myFile.txt"
	int i;
	string svalue;
	for(i=0;i<40;i++){
		sprintf(svalue,"%3d ",readings[i]);
		WriteString(myFileHandle, IOResult, svalue);
	}
	Close(myFileHandle, IOResult);
}
