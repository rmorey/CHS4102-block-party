int threshold;
int range;
int direction;
int readings[20]; // to store readings
int rsigns[20];
int step,nstep;
float filteredEnc;
float avgP,avgM;
int s_target;
int s_targetinit;

void tdisplay(int arg=0){
	nxtDisplayBigTextLine(1, "%d", arg);
	nxtDisplayBigTextLine(3, "%d", nMotorEncoder[M_lightsensor]);
  nxtDisplayBigTextLine(5, "%d", SensorValue[S_lightsensor]);
}

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
		power=abs(30*(threshold-rval))/range;
		if(power>15) power=15;
		sign=threshold>rval?direction:-direction;
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
	//sets up threshold, range and direction parameters for edge tracking


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
	threshold=(rmin+rmax)/2;
	range=rmax-rmin;

	//get averages for better threshold
	avgM=0;avgP=0;cntM=0;cntP=0;
	for(i=0;i<nstep;i++){
		rsigns[i]=readings[i]<threshold?-1:1;
		if(rsigns[i]<0) {avgM+=readings[i]; cntM++; }
		else { avgP+=readings[i]; cntP++;}
	}
	avgM/=cntM;
	avgP/=cntP;
	threshold=(int)floor(0.5+(avgM+avgP)/2);
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
	while(direction*(threshold-readings[i])>0) i++;

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
	threshold=(rmax+avgM)/2; //adjustment for thin line (average of peak can be depressed)
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
	threshold=(rmax+rmin)/2;
	range=rmax-rmin;
	return(maxloc);//returns location of max
}
