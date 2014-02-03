
void oldDriveUpdate(){
   motor[M_DriveFL] = (100*drivejx1)/142+(100*drivejy1)/142+drivejx2;
   motor[M_DriveBR] = (100*drivejx1)/142+(100*drivejy1)/142-drivejx2;
   motor[M_DriveFR] = (100*drivejx1)/142-(100*drivejy1)/142+drivejx2;
   motor[M_DriveBL] = (100*drivejx1)/142-(100*drivejy1)/142-drivejx2;
}

int scale(int tt){
	float mag;
	int sign,val;
	mag=abs(tt);
	if(mag<7) return 0;
	sign=tt>0?1:-1;
	val=(int)floor(mag/4.0+3.0*mag*mag/400.0);
	if(val>90) val=100;
	return(sign*mag);
}
void DriveUpdate(){
   motor[M_DriveFL] = scale((100*drivejx1)/142+(100*drivejy1)/142+drivejx2);
   motor[M_DriveBR] = scale((100*drivejx1)/142+(100*drivejy1)/142-drivejx2);
   motor[M_DriveFR] = scale((100*drivejx1)/142-(100*drivejy1)/142+drivejx2);
   motor[M_DriveBL] = scale((100*drivejx1)/142-(100*drivejy1)/142-drivejx2);
}
/*
void BDriveUpdate(){
   motor[M_DriveFL] = scale((100*drivejx1)/142+(100*drivejy1)/142+drivejx2)/12;
   motor[M_DriveBR] = scale((100*drivejx1)/142+(100*drivejy1)/142-drivejx2);
   motor[M_DriveFR] = scale((100*drivejx1)/142-(100*drivejy1)/142-drivejx2)/12;
   motor[M_DriveBL] = scale((100*drivejx1)/142-(100*drivejy1)/142+drivejx2);
}

void LDriveUpdate(){
   motor[M_DriveFL] = scale((100*drivejx1)/142+(100*drivejy1)/142+drivejx2);
   motor[M_DriveBR] = 0;
   motor[M_DriveFR] = 0;
   motor[M_DriveBL] = scale((100*drivejx1)/142-(100*drivejy1)/142+drivejx2);
}
void RDriveUpdate(){
   motor[M_DriveFL] = 0;
   motor[M_DriveBR] = scale((100*drivejx1)/142+(100*drivejy1)/142-drivejx2);
   motor[M_DriveFR] = scale((100*drivejx1)/142-(100*drivejy1)/142-drivejx2);
   motor[M_DriveBL] = 0;
}*/

void DriveWait1Msec(int time){ //waiting and driving only
	ClearTimer(T4);
	while(time1(T4)<time){
		getJoyValues();
   	DriveUpdate();
	}
}

int DriveWaitAbort1Msec(int time){ //waiting and driving only
	ClearTimer(T4);
	while(time1(T4)<time){
		getJoyValues();
		if(driveButton9) return -1; //abort flag
   	DriveUpdate();
	}
	return 0;
}
