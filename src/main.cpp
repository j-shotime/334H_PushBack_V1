#include "everything.h"

void pre_auton(void)
{
  SetupHDrive(9, leftMotors, rightMotors, strafe);
}

void autonomous(void)
{

}

void usercontrol(void)
{
  while (true)
  {
    SetDrive(Controller.Axis3.position(percent), Controller.Axis2.position(percent), Controller.Axis1.position(percent) + Controller.Axis4.position(percent));
    if(Controller.ButtonR1.pressing()) intake.spin(fwd,12000,voltageUnits::mV);
    else if(Controller.ButtonR2.pressing()) intake.spin(fwd,-12000,voltageUnits::mV);
    else intake.spin(fwd,0,voltageUnits::mV);

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