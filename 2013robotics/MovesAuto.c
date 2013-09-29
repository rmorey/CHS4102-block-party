//*****************************************************************
// Autonomous program
// Distances: 775 for each additional foot
// 1700 Msec for 24 in
// 6350 Msec for 8 ft = 1700 + 775*6
// 10ft would be 6350+2*775 = 7900
//*****************************************************************

//DRIVE ROUTINES
long nNumbCyles;
long nInits = 0;
#include "CHSGyroDriver.c"
#include "drivers/hitechnic-irseeker-v2.h"

typedef enum {slow, medium, fast} speedcontrol;

float fOldAngle = -1;

void gyroFlasher()
{
  bool bFlasher = false;
  while (!bIsGyroInitialized())
  {
    if (bIsGyroInitialized())
      break;

    //
    // Some flashy graphics.
    //
    if (bFlasher)
    {
      nxtDisplayBigStringAt(0, 23, " Startup ");
      bFlasher = false;
    }
    else
    {
      nxtDisplayBigStringAt(0, 23, "        ");
      bFLasher = true;
    }
    wait1Msec(250);// Wait for the gyro to initialize. This takes a couple of seconds.
  }
}


//////////////////////////
//Drive nudging
//positive is forward
void autodrive(int xpow,int ypow,int duration)
{
	motor[DriveFL] = xpow + ypow;
  motor[DriveBR] = xpow + ypow;
	motor[DriveFR] = xpow - ypow;
  motor[DriveBL] = xpow - ypow;
  wait1Msec(duration);
  motor[DriveFL] = 0;
 	motor[DriveBR] = 0;
  motor[DriveFR] = 0;
  motor[DriveBL] = 0;
}

void goForward(int time){
  autodrive(100,0,time);
}
void goBack(int time){
  autodrive(-100,0,time);
}

void goRight(int time){
  autodrive(0,100,time);
}

void goLeft(int time){
  autodrive(0,-100,time);
}

void distForward(float dist, speedcontrol spd){ //distance in inches
  int power;
  float time, powerfactor;
  switch (spd)
  {
    case medium:
      power = 40;
      time=200 + dist*48.0;
    break;
    case fast:
      power = 100;
      time= 100+ dist*36.5;
    break;
    case slow:
      power = 25;
      time = 250 + dist*67.0;
    break;
 }
 float batvolt = externalBattery;
 if(batvolt==-1) powerfactor=1;
 else
 {
   powerfactor=12200.0/batvolt;
 }
 autodrive(power, 0,(int)floor(time*powerfactor));
}

void distBackward(float dist, speedcontrol spd){
  int power;
  float time, powerfactor;
  switch (spd)
  {
    case medium:
      power = 40;
      time=200 + dist*48.0;
    break;
    case fast:
      power = 100;
      time= 100+ dist*36.5;
    break;
    case slow:
      power = 25;
      time = 250 + dist*67.0;
    break;
  }
  float batvolt = externalBattery;
 if(batvolt==-1) powerfactor=1;
 else
 {
   powerfactor=12200.0/batvolt;
 }
  autodrive(-power,0,(int)floor(time*powerfactor));
}

void distRight(float dist, speedcontrol spd){
  int power;
  float time, powerfactor;
  switch (spd)
  {
    case medium:
      power = 40;
      time=200 + dist*48.0;
    break;
    case fast:
      power = 100;
      time= 100+ dist*36.5;
    break;
    case slow:
      power = 25;
      time = 250 + dist*67.0;
    break;
  }
  float batvolt = externalBattery;
 if(batvolt==-1) powerfactor=1;
 else
 {
   powerfactor=12200.0/batvolt;
 }
  autodrive(0,power,(int)floor(time*powerfactor));
}

void distLeft(float dist, speedcontrol spd){
  float time, powerfactor;
  int power;
  switch (spd)
  {
    case medium:
      power = 40;
      time=200 + dist*48.0;
    break;
    case fast:
      power = 100;
      time= 100+ dist*36.5;
    break;
    case slow:
      power = 25;
      time = 250 + dist*67.0;
    break;
  }
  float batvolt = externalBattery;
 if(batvolt==-1) powerfactor=1;
 else
 {
   powerfactor=12200.0/batvolt;
 }
  autodrive(0,-power,(int)floor(time*powerfactor));
}

void turnTo(float deg, int power){
  while(deg-10 <= fGyroAngle || fGyroAngle <= deg+10){
  	fOldAngle = fGyroAngle;
  	if(abs(deg-fGyroAngle) < abs(fGyroAngle-deg)){
    	motor[DriveFL] = 50;
 	  	motor[DriveBR] = 50;
   		motor[DriveFR] = 50;
    	motor[DriveBL] = 50;
    }
    if(abs(deg-fGyroAngle) > abs(fGyroAngle-deg)){
    	motor[DriveFL] = -50;
 	  	motor[DriveBR] = -50;
   		motor[DriveFR] = -50;
    	motor[DriveBL] = -50;
    }
    if(fGyroAngle == deg){
    	break;
    }
    //nxtDisplayBigStringAt(0, 23, "  %3d   ", (long) fGyroAngle);
  }
   motor[DriveFL] = 0;
 	 motor[DriveBR] = 0;
 	 motor[DriveFR] = 0;
   motor[DriveBL] = 0;
}

void turnLeftTo(float deg, int power){
  while(fGyroAngle<=-deg){
    motor[DriveFL] = -50;
 	  motor[DriveBR] = -50;
   	motor[DriveFR] = -50;
    motor[DriveBL] = -50;
    //nxtDisplayBigStringAt(0, 23, "  %3d   ", (long) fGyroAngle);
    fOldAngle = fGyroAngle;
  }
  motor[DriveFL] = 0;
 	motor[DriveBR] = 0;
  motor[DriveFR] = 0;
  motor[DriveBL] = 0;
}

void anglereading(){
  while(fGyroAngle != fOldAngle){
    nxtDisplayBigStringAt(0, 23, "  %3d   ", (long) fGyroAngle);
    fOldAngle = fGyroAngle;
  }
}

void IRsense()
{
	int _dirDC = 0;
  int _dirAC = 0;
	int dcS1, dcS2, dcS3, dcS4, dcS5 = 0;
  tHTIRS2DSPMode _mode = DSP_1200;
  int e;

  nNumbCyles = 0;
  ++nInits;
  while (true)
  {
   if ((nNumbCyles & 0x04) == 0)
    wait1Msec(1);
   else
    wait1Msec(1);

  	// set the DSP to the new mode
   if (HTIRS2setDSPMode(HTIRS2, _mode))
    break; // Sensor initialized

   ++nNumbCyles;
   }
   while (true)
    {
      ++nNumbCyles;
      // Read the current non modulated signal direction
      _dirDC = HTIRS2readDCDir(HTIRS2);
      if (_dirDC < 0)
        break; // I2C read error occurred

      // Read the individual signal strengths of the internal sensors
      // Do this for both unmodulated (DC) and modulated signals (AC)
      if (!HTIRS2readAllDCStrength(HTIRS2, dcS1, dcS2, dcS3, dcS4, dcS5))
        break; // I2C read error occurred
    	if(dcS2>10)
     	{
 	  		e=1;
     	}
			if(dcS3>10)
			{
				e=2;
			}
			if(dcS4>10)
			{
				e=3;
			}
    }

}
