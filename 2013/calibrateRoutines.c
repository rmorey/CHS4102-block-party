void displayLiftEncoder(){
  	nxtDisplayBigTextLine(1, "PL ENC");
  	nxtDisplayBigTextLine(3, "%d",nMotorEncoder[Mtr_ParaLift]);
}


void displayPullSlider(){
  	nxtDisplayBigTextLine(1, "P %d",ServoValue[PullSliderServo]);
  	nxtDisplayBigTextLine(3, "R %d",ServoValue[RotServo]);
}
