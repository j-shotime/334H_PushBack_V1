#include "everything.h"

// Intake control helper functions
inline void intakeSpin(double power = -1.0) {
  // power range: -1.0 = full intake, 1.0 = full outtake
  intake.spin(fwd, 12000 * power, voltageUnits::mV);
}

inline void intakeStop(brakeType mode = coast) {
  intake.stop(mode);
}

// Optional quick aliases for readability
inline void intakeIn(double power = 1.0) {
  intakeSpin(-power);  // negative = intake direction
}

inline void intakeOut(double power = 1.0) {
  intakeSpin(power);   // positive = outtake direction
}

bool thing = false;
autonType autontype = autonType::blue;
int autonNumber = 7;

void pre_auton(void)
{
    SetupHDrive(PORT10, 48, 72, 4, 1, 1, 2.75, leftMotors, rightMotors, strafe);
    Brain.Screen.render(true, true);
    // Prevent main from exiting with an infinite loop.
    while (true)
    {
        Controller.Screen.newLine();
        Controller.Screen.print(Inertial.rotation(degrees));
        Brain.Screen.clearScreen();
        Brain.Screen.setFillColor(purple);
        Brain.Screen.drawRectangle(11, 10, 146, 35);
        Brain.Screen.setCursor(2, 6);
        Brain.Screen.print("Skills");
        Brain.Screen.setFillColor(red);
        Brain.Screen.drawRectangle(167, 10, 146, 35);
        Brain.Screen.setCursor(2, 23);
        Brain.Screen.print("Red");
        Brain.Screen.setFillColor(blue);
        Brain.Screen.drawRectangle(323, 10, 146, 35);
        Brain.Screen.setCursor(2, 38);
        Brain.Screen.print("Blue");
        Brain.Screen.drawLine(11, 55, 469, 55);
        switch (autontype)
        {
        case autonType::skills:
            Brain.Screen.setFillColor(purple);
            break;

        case autonType::red:
            Brain.Screen.setFillColor(red);
            break;

        case autonType::blue:
            Brain.Screen.setFillColor(blue);
            break;
        }
        if (autontype == autonType::skills)
        {
            Brain.Screen.drawRectangle(11, 65, 460, 160);
        }
        else
        {
            Brain.Screen.setPenColor(black);
            if (autonNumber == 1)
                Brain.Screen.setPenColor(white);
            Brain.Screen.drawRectangle(11, 65, 225, 33);
            Brain.Screen.setPenColor(black);
            if (autonNumber == 2)
                Brain.Screen.setPenColor(white);
            Brain.Screen.drawRectangle(246, 65, 225, 33);
            Brain.Screen.setPenColor(black);
            if (autonNumber == 3)
                Brain.Screen.setPenColor(white);
            Brain.Screen.drawRectangle(11, 108, 225, 33);
            Brain.Screen.setPenColor(black);
            if (autonNumber == 4)
                Brain.Screen.setPenColor(white);
            Brain.Screen.drawRectangle(246, 108, 225, 33);
            Brain.Screen.setPenColor(black);
            if (autonNumber == 5)
                Brain.Screen.setPenColor(white);
            Brain.Screen.drawRectangle(11, 151, 225, 33);
            Brain.Screen.setPenColor(black);
            if (autonNumber == 6)
                Brain.Screen.setPenColor(white);
            Brain.Screen.drawRectangle(246, 151, 225, 33);
            Brain.Screen.setPenColor(black);
            if (autonNumber == 7)
                Brain.Screen.setPenColor(white);
            Brain.Screen.drawRectangle(11, 194, 225, 33);
            Brain.Screen.setPenColor(black);
            if (autonNumber == 8)
                Brain.Screen.setPenColor(white);
            Brain.Screen.drawRectangle(246, 194, 225, 33);
            Brain.Screen.setPenColor(white);

            Brain.Screen.printAt(22, 86, "- 4 balls tioli");
            // Brain.Screen.printAt(258, 86, "- | 2 Ring | 1 Stake");
            // Brain.Screen.printAt(22, 129, "+ | 2 Ring | 1 Stake");
            // Brain.Screen.printAt(258, 129, "+ | 0 Ring |   Rush");
        }

        if (Brain.Screen.pressing())
        {
            if (Brain.Screen.yPosition() > 10 && Brain.Screen.yPosition() < 45)
            {
                if (Brain.Screen.xPosition() > 11 && Brain.Screen.xPosition() < 157)
                    autontype = autonType::skills;
                else if (Brain.Screen.xPosition() > 167 && Brain.Screen.xPosition() < 313)
                    autontype = autonType::red;
                else if (Brain.Screen.xPosition() > 323 && Brain.Screen.xPosition() < 469)
                    autontype = autonType::blue;
            }
            else
            {

                if (withinRect(Brain.Screen.xPosition(), Brain.Screen.yPosition(), 11, 65, 225, 33))
                    autonNumber = 1;
                else if (withinRect(Brain.Screen.xPosition(), Brain.Screen.yPosition(), 246, 65, 225, 33))
                    autonNumber = 2;
                else if (withinRect(Brain.Screen.xPosition(), Brain.Screen.yPosition(), 11, 108, 225, 33))
                    autonNumber = 3;
                else if (withinRect(Brain.Screen.xPosition(), Brain.Screen.yPosition(), 246, 108, 225, 33))
                    autonNumber = 4;
                else if (withinRect(Brain.Screen.xPosition(), Brain.Screen.yPosition(), 11, 151, 225, 33))
                    autonNumber = 5;
                else if (withinRect(Brain.Screen.xPosition(), Brain.Screen.yPosition(), 246, 151, 225, 33))
                    autonNumber = 6;
                else if (withinRect(Brain.Screen.xPosition(), Brain.Screen.yPosition(), 11, 194, 225, 33))
                    autonNumber = 7;
                else if (withinRect(Brain.Screen.xPosition(), Brain.Screen.yPosition(), 246, 194, 225, 33))
                    autonNumber = 8;
            }
        }

        Brain.Screen.render();
        wait(100, msec);
    }
}

void autonomous(void)
{

    Brake(brakeType::hold);
    // isDriver = false;
    // hasShortBuzz = false;
    // hasLongBuzz = false;
    // while (Drive.I.isCalibrating())
    wait(20, msec);
    // if (autontype == autonType::red)
    // 	 tM = 1;
    // else
    // 	tM = -1;
    if (autontype == autonType::skills)
    {
        // skills();
    }
    else
    {

        if (autonNumber == 1)
        {
            Shift(0, 100, 0.00, 2, 0.5, 45.5, 90, 0.00, 24, 0.7);
            descore.set(!descore.value());
            RightSwing(90, 40, 0, 45);
            Shift(0, 100, 0.00, 2, 0.5, 12.5, 100, 30, 24, 0.8);
            SetDrive(50, 50, 20);
            intake.spin(fwd, -12000, voltageUnits::mV);
            wait(0.5, sec);
            SetDrive(-10, -10, -20);
            wait(0.1, sec);
            SetDrive(50, 50, 20);
            wait(0.1, sec);
            SetDrive(-10, -10, -20);
            wait(0.3, sec);
            SetDrive(50, 50, 20);
            wait(0.1, sec);
            SetDrive(-10, -10, -20);
            wait(0.3, sec);
            SetDrive(50, 50, 20);
            wait(0.5, sec);
            Shift(0, 100, 0.00, 2, 0.5, -29, 100, 0.00, 24, 0.8);
            indexer.open();
        }
        else if (autonNumber == 2)
        {
            Shift(0, 100, 0.00, 2, 0.5, 47, 90, 0.00, 24, 0.8);
            descore.set(!descore.value());
            //LeftSwing(-90, 40, 0, 45);
            Shift(0, 100, 0, 2, 0.5, 12.5, 100, 30, 24, 0.8);
            SetDrive(50, 50, 20);
            intake.spin(fwd, -12000, voltageUnits::mV);
            wait(0.5, sec);
            SetDrive(-10, -10, -20);
            wait(0.1, sec);
            SetDrive(50, 50, 20);
            wait(0.1, sec);
            SetDrive(-10, -10, -20);
            wait(0.3, sec);
            SetDrive(50, 50, 20);
            wait(0.1, sec);
            SetDrive(-10, -10, -20);
            wait(0.3, sec);
            SetDrive(50, 50, 20);
            wait(0.5, sec);
            Shift(0, 100, 0.00, 2, 0.5, -29, 100, 0.00, 24, 0.8);
            indexer.open();
        }
        else if (autonNumber == 3)
        {
            intake.spin(fwd, -12000, voltageUnits::mV);
            Shift(10, 100, 0, 2, 0.5, 30, 100, 30, 24, 0.8);
            Turn(-45, 60, 10, 180);
            Shift(-4, 100, 0.00, 2, 0.5, 20, 100, 30, 22, 0.8);
            wait(0.5, sec);
            intake.spin(fwd, 12000, voltageUnits::mV);
        }
        else if (autonNumber == 4)
        {
            intake.spin(fwd, -12000, voltageUnits::mV);
            Shift(-10, 100, 0, 2, 0.5, 30, 100, 30, 24, 0.8);
            RightSwing(90, 60, 10, 90);
            Shift(9, 100, 0.00, 2, 0.5, -12, 100, 30, 22, 0.8);
            RightSwing(90, 60, 10, 90);
        }
        else if (autonNumber == 5)
        {
            Shift(0, 100, 0, 2, 0.5, 36, 60, 10, 24, 0.8);
            Turn(-90, 60, 10, 180);
            descore.set(!descore.value());
            Shift(0, 100, 0, 2, 0.5, 3, 60, 25, 24, 0.8);
            SetDrive(50, 50, 0);
            intakeIn();
            wait(0.5, sec);
            SetDrive(-10, -10, 0);
            wait(0.1, sec);
            SetDrive(50, 50, 0);
            wait(0.1, sec);
            SetDrive(-10, -10, 0);
            wait(0.1, sec);
            SetDrive(50, 50, 0);
            wait(0.5, sec);
            SetDrive(-30, -30, 0);
            //antidescore.set(!antidescore.value());
            descore.set(!descore.value());
            wait(3, sec);
            SetDrive(0,0,0);
            indexer.open();
            //Shift(1, 100, 0, 2, 0.5, -30, 100, 10, 24, 0.8);
        }
        else if (autonNumber == 6)
        {
            Shift(0, 100, 0, 2, 0.5, 10, 60, 10, 24, 0.8);
        }
        else if (autonNumber == 7)
        {
            Shift(-5.2, 100, 0, 2, 0.5, 38, 40, 10, 24, 0.8);
            intakeOut(0.6);
            wait(1, sec);
            Shift(0, 40, 0, 2, 0.5, -35, 40, 0, 12, 0.8);
            Shift(11, 100, 0, 2, 0.5, 0, 40, 0, 12, 0.8);
            intakeIn();
            Shift(0, 100, 0, 2, 0.5, 38, 40, 10, 24, 0.8);
            Shift(0, 100, 0, 2, 0.5, -30, 40, 10, 24, 0.8);
            Shift(16, 100, 0, 2, 0.5, 0, 40, 0, 12, 0.8);
            Turn(180, 20, 10, 90);
            SetDrive(-30,-30);
            wait(0.5, sec);
            indexer.open();
        }
        else if (autonNumber == 8)
        {
            Shift(5.2, 100, 0, 2, 0.5, 38, 40, 10, 24, 0.8);
            wait(1, sec);
            Shift(0, 40, 0, 2, 0.5, -35, 40, 0, 12, 0.8);
            Shift(-11, 100, 0, 2, 0.5, 0, 40, 0, 12, 0.8);
            intakeIn();
            Shift(0, 100, 0, 2, 0.5, 38, 40, 10, 24, 0.8);
            Shift(0, 100, 0, 2, 0.5, -30, 40, 10, 24, 0.8);
            Shift(-16, 100, 0, 2, 0.5, 0, 40, 0, 12, 0.8);
            Turn(-180, 20, 10, 90);
            SetDrive(-30,-30);
            wait(0.5, sec);
            indexer.open();
        }
    }
}

void usercontrol(void)
{
    while (true)
    {
        SetDrive(Controller.Axis3.value(), Controller.Axis2.value(), Controller.Axis1.value() + Controller.Axis4.value());
        if (Controller.ButtonR1.pressing())
            intakeOut();
        else if (Controller.ButtonR2.pressing() || Controller.ButtonL2.pressing())
            intakeIn();
        else
            intake.spin(fwd, 0, voltageUnits::mV);
        if (Controller.ButtonL1.PRESSED)
        {
            descore.set(!descore.value());
            if(thing) descore.set(descore.value());
        }
        if(Controller.ButtonDown.PRESSED)
        {
            matchLoader.set(!matchLoader.value());
        }
        if (Controller.ButtonL2.pressing())
            indexer.open();
        else
            indexer.close();
        if(Controller.ButtonB.pressing())
        {
            intakeOut(0.5);
        }
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
    //vexMotorPwmSet(,);
}