int  drivejx1,drivejx2,drivejy1,drivejy2,driveButtons,driveTopHat;
int  armjx1,armjx2,armjy1,armjy2,armTopHat,armButtons;
bool armButton1,armButton2,armButton3,armButton4,armButton5;
bool armButton6,armButton7,armButton8,armButton9,armButton10;
bool driveButton1,driveButton2,driveButton3,driveButton4,driveButton5;
bool driveButton6,driveButton7,driveButton8,driveButton9,driveButton10;

void getJoyValues(){
  getJoystickSettings(joystick);

  //DRIVING JOYSTICK VALUES
 drivejx1=(100*joystick.joy1_x1)/127;
 drivejx2=(100*joystick.joy1_x2)/127;
 drivejy1=(100*joystick.joy1_y1)/127;
 drivejy2=(100*joystick.joy1_y2)/127;
 driveButtons=joystick.joy1_Buttons;
 driveTopHat =joystick.joy1_TopHat;

 driveButton1=((driveButtons&1)>0);driveButton2=((driveButtons&2)>0);driveButton3=((driveButtons&4)>0);driveButton4=((driveButtons&8)>0);driveButton5=((driveButtons&16)>0);
 driveButton6=((driveButtons&32)>0);driveButton7=((driveButtons&64)>0);driveButton8=((driveButtons&128)>0);driveButton9=((driveButtons&256)>0);driveButton10=((driveButtons&512)>0);

 //ARM JOYSTICK VALUES
 armTopHat =joystick.joy2_TopHat;
 armButtons=joystick.joy2_Buttons;
 armjx1= (100*joystick.joy2_x1)/127;
 armjx2= (100*joystick.joy2_x2)/127;
 armjy1= (100*joystick.joy2_y1)/127;
 armjy2= (100*joystick.joy2_y2)/127;


 armButton1=((armButtons&1)>0);armButton2=((armButtons&2)>0);armButton3=((armButtons&4)>0);armButton4=((armButtons&8)>0);armButton5=((armButtons&16)>0);
 armButton6=((armButtons&32)>0);armButton7=((armButtons&64)>0);armButton8=((armButtons&128)>0);armButton9=((armButtons&256)>0);armButton10=((armButtons&512)>0);

    if(abs(drivejx1)<10) drivejx1 = 0;
   if(abs(drivejx2)<10) drivejx2 = 0;
   if(abs(drivejy1)<10) drivejy1 = 0;
   if(abs(drivejy2)<10) drivejy2 = 0;
   if(abs(armjx1)<10) armjx1 = 0;
   if(abs(armjx2)<10) armjx2 = 0;
   if(abs(armjy1)<10) armjy1 = 0;
   if(abs(armjy2)<10) armjy2 = 0;

 }
