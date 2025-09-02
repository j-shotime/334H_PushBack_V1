#include "everything.h"

void pre_auton(void)
{
  SetupHDrive(PORT11, 48, 72, 4, 24, 48, 2.75, leftMotors, rightMotors, strafe);
}

void autonomous(void)
{
  Brake(brakeType::hold);
  Controller.Screen.print("1");
  Shift(0, 100, 0.00, 2, 0.5, 45.5, 50, 0.00, 24, 0.9);
  matchLoader.set(!matchLoader.value());
  RightSwing(90, 40, 0, 45);
  Shift(0, 100, 0.00, 2, 0.5, 12.5, 100, 30, 24, 0.8);
  SetDrive(50,50,20);
  intake.spin(fwd,-12000,voltageUnits::mV);
  wait(0.5, sec);
  SetDrive(-10,-10,-20);
  wait(0.1, sec);
  SetDrive(50,50,20);
   wait(0.1, sec);
  SetDrive(-10,-10,-20);
  wait(0.3, sec);
  SetDrive(50,50,20);
  wait(0.1, sec);
  SetDrive(-10,-10,-20);
  wait(0.3, sec);
  SetDrive(50,50,20);
  wait(0.5, sec);
  Shift(0, 100, 0.00, 2, 0.5, -29, 100, 0.00, 24, 0.8);
  indexer.open();
}

void usercontrol(void)
{
  while (true)
  {
    SetDrive(Controller.Axis3.value(), Controller.Axis2.value(), Controller.Axis1.value() + Controller.Axis4.value());
    if(Controller.ButtonR1.pressing()) intake.spin(fwd,12000,voltageUnits::mV);
    else if(Controller.ButtonR2.pressing() || Controller.ButtonL2.pressing()) intake.spin(fwd,-12000,voltageUnits::mV);
    else intake.spin(fwd,0,voltageUnits::mV);
    if(Controller.ButtonL1.PRESSED) 
    {
      matchLoader.set(!matchLoader.value());
    }
    if(Controller.ButtonL2.pressing()) indexer.open();
    else indexer.close();
    wait(20, msec);
  }
}

int main()
{
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true)
  {
    wait(100, msec);
  }
}