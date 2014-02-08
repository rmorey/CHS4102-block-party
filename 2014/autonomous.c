#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S3,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     SONAR,          sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     M_DRIVE_BL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     M_DRIVE_FL,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     M_BELT,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     M_LIFT_L,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     M_DRIVE_BR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     M_DRIVE_FR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C3_1,     M_LIFT_R,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C3_2,     M_FLAG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    SV_SCORE,              tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_2,    SV_HOOK,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_3,    SV_LID,                tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    SV_AUTO,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "robot.h"

#define PWR_SCAN 30
#define ENC_LAST_GOAL 10000 //distance to last goal TODO: fix this

bool start_on_right;
bool ramp;
bool block;
bool wait;
TButtons nBtn;
int pwr_x = 0;
int pwr_y = 0;

void getButton() { //gets next button press
  while (true) // wait for button press
  {
    nBtn = nNxtButtonPressed;
    if (nBtn != -1)
      break;
  }
  PlaySoundFile("! Click.rso");
  wait1Msec(500);

}

task main()
{
  nNxtButtonTask  = -2; //hijacks button control
  nNxtExitClicks  = 1;
  //disableDiagnosticsDisplay();
  hogCPU();
  eraseDisplay();
  nxtDisplayCenteredTextLine(0, "Which side?");
  nxtDisplayCenteredTextLine(7, "Left     Right");
  while (true) // wait for button press
  {
    nBtn = nNxtButtonPressed;
    if (nBtn != -1)
      break;
  }
  wait1Msec(500);

  switch (nBtn) {
  case kRightButton:
    start_on_right = true;
    break;
  case kLeftButton:
    start_on_right = false;
    break;
  default:
    break;
  }

  eraseDisplay();
  nxtDisplayCenteredTextLine(0, "Block, ramp, or both?");
  nxtDisplayCenteredTextLine(7, "Block Both Ramp");
  getButton();

  switch (nBtn) {
  case kRightButton:
    ramp = true;
    block = false;
    break;
  case kLeftButton:
    ramp = false;
    block = true;
    break;
  case kEnterButton:
    ramp = true;
    block = true;
    break;
  default:
    StopAllTasks();
  }

  eraseDisplay();
  nxtDisplayCenteredTextLine(0, "Wait for Start?");
  nxtDisplayCenteredTextLine(7, "Yes    No");
  getButton();

  switch (nBtn) {
  case kRightButton:
    wait = false;
    break;
  case kLeftButton:
    wait = true;
    break;
  default:
    StopAllTasks();
  }

  eraseDisplay();
  nxtDisplayCenteredTextLine(0, "Ready?");
  nxtDisplayCenteredTextLine(7, "Go!");
  getButton();

  switch (nBtn) {
  case kEnterButton:
    break;
  default:
    StopAllTasks();
  }

  /////////////////////////////////////////////////////////
  ////code that actually runs during match starts here/////
  /////////////////////////////////////////////////////////
  eraseDisplay();
  releaseCPU();
  eraseDisplay();
  wait1Msec(50);
  if (wait) waitForStart();

  ClearTimer(T1); //keeps track of autonomous period, 30000 ms long

  if (ramp && !block) {
    //just get on ramp
    goForward(100);
    wait1Msec(900);
    if (start_on_right) goLeft(100); else goRight(100);
    wait1Msec(2100);
    driveStop();
  }

  else {
    //score block in IR goal
    if (start_on_right) pwr_x = -PWR_SCAN; else pwr_x = PWR_SCAN;
    driveTilted(pwr_x, pwr_y);
    while (SensorValue[SONAR] < 100) { //stops at edge of thing, TODO: make this use encoders
      if (SensorValue[IR] == 5) { //we see the IR beacon
        driveStop();
        servo[SV_AUTO] = 250;
        wait1Msec(1000);
        servo[SV_AUTO] = 0;
        break;

      }
      int err = 28 - SensorValue[SONAR]; //10cm is our target distance
      if (abs(err) > 10) {
        pwr_y = 0; //ignore if error is really big, we're probably doing something wrong
      }
      else {
        pwr_y = -2*err; //try to correct for error, maybe change this value
      }
      driveTilted(pwr_x, pwr_y);
    }

    if (ramp) {

      //go remaining distance TODO: return on closest side
      if (start_on_right)
        pwr_x = -2.5*PWR_SCAN;
      else
        pwr_x = 2.5*PWR_SCAN;

      driveTilted(pwr_x, pwr_y);
      while(SensorValue[SONAR] < 50) {  //TODO: make this use encoders
        int err = 10 - SensorValue[SONAR]; //10cm is our target distance
        if (abs(err) > 10)
          pwr_y = 0; //ignore if error is really big
        else
          pwr_y = -2*err; //try to correct for error
        driveTilted(pwr_x, pwr_y);
      }
      driveStop();

      //get on ramp

      goForward(100);
      wait1Msec(900);

      if (start_on_right) goRight(100); else goLeft(100);
      wait1Msec(2100);

      driveStop();
    }
  }
}
