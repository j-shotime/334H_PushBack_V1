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
autonType autontype = autonType::skills;
int autonNumber = 3;

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
    wait(20, msec);
    if (autontype == autonType::skills)
    {
        SetDrive(-100, -100, 0);
        wait(300, msec);
        matchLoader.open();
        SetDrive(100, 100, 0);
        wait(1500, msec);
        Brake();
        // Shift(-10, 100, 20, 5, 1, 26, 100, 20, 18, 0.8);
        // LeftSwing(90, 50, 10, 90);
        // intakeIn();
        // matchLoader.open();
        // wait(500, msec);
        // SetDrive(60, 60, 0);
        // wait(2000, msec);
        // Shift(-1, 100, 20, 5, 1, -24, 100, 20, 18, 0.8);
        // SetDrive(-50, -50, -4);
        // wait(800, msec);
        // intakeIn();
        // indexer.open();
        // wait(2500, msec);
        // indexer.close();
        // matchLoader.close();
        // LeftSwing(135, 50, 10, 24);
        // Shift(-2, 100, 20, 5, 1, 42, 100, 20, 24, 0.8);
        // intakeOut();
        // wait(1000, msec);
        // intakeStop();
        // Shift(0, 100, 20, 5, 1, -10, 100, 20, 24, 0.8);
        // Turn(-45, 50, 10, 24);
        // intakeIn();
        // Shift(0, 100, 20, 5, 1, 48, 100, 20, 24, 0.8);
        // Turn(-45, 50, 10, 24);
        // indexer.open();
        // wait(1000, msec);
        // indexer.close();
        // Shift(0, 100, 20, 5, 1, 48, 100, 20, 18, 0.8);
        // RightSwing(-45, 50, 10, 90);
        // intakeIn();
        // matchLoader.open();
        // wait(1000, msec);
        // SetDrive(60, 60, 0);
        // Shift(-1, 100, 20, 5, 1, -24, 100, 20, 18, 0.8);
        // SetDrive(-50, -50, 4);
        // wait(2000, msec);
        // intakeIn();
        // indexer.open();
    }
    else
    {
        if (autonNumber == 1)
        {
            Shift(-10, 100, 20, 5, 1, 26, 100, 20, 18, 0.8);
            LeftSwing(90, 50, 10, 90);
            matchLoader.open();
            wait(500, msec);
            SetDrive(60, 60, 0);
            wait(1000, msec);
            Shift(-1, 100, 20, 5, 1, -24, 100, 20, 18, 0.8);
            SetDrive(-50, -50, -4);
            wait(800, msec);
            intakeIn();
            indexer.open();
            wait(2500, msec);
            indexer.close();
            matchLoader.close();
            LeftSwing(135, 50, 10, 24);
            Shift(-2, 100, 20, 5, 1, 42, 100, 20, 24, 0.8);
            intakeOut(0.6);
            wait(1000, msec);
            intakeStop();
            Shift(0, 100, 20, 5, 1, -10, 100, 20, 24, 0.8);
            Turn(-45, 50, 10, 24);
            intakeIn();
            Shift(0, 100, 20, 5, 1, 48, 100, 20, 24, 0.8);
            Turn(-45, 50, 10, 24);
            indexer.open();
        }
        else if (autonNumber == 2)
        {
            Shift(10, 100, 20, 5, 1, 26, 100, 20, 18, 0.8);
            RightSwing(-90, 50, 10, 90);
            matchLoader.open();
            wait(500, msec);
            SetDrive(60, 60, 0);
            wait(1000, msec);
            Shift(1, 100, 20, 5, 1, -24, 100, 20, 18, 0.8);
            intakeIn();
            matchLoader.close();
            SetDrive(-50, -50, 4);
            wait(800, msec);
            indexer.open();
            wait(2500, msec);
            indexer.close();
            RightSwing(-135, 50, 10, 24);
            Shift(2, 100, 15, 5, 1, 42, 100, 20, 24, 0.8);
            Turn(180, 50, 10, 24);
            indexer.open();
            wait(1000, msec);
            Turn(-135, 50, 10, 24);
            indexer.close();
            Shift(0, 100, 20, 5, 1, 48, 100, 20, 24, 0.8);
            Turn(-135, 50, 10, 24);
            Shift(0, 100, 20, 5, 1, 10, 100, 20, 24, 0.8);
            intakeOut(0.6);
        }
        else if (autonNumber == 3)
        {
            Shift(-10, 100, 20, 5, 1, 26, 100, 20, 18, 0.8);
            LeftSwing(90, 50, 10, 90);
            matchLoader.open();
            wait(500, msec);
            SetDrive(60, 60, 0);
            wait(1000, msec);
            intakeIn();
            wait(100, msec);
            intakeIn();
            Shift(-1, 100, 20, 5, 1, -24, 100, 20, 18, 0.8);
            matchLoader.close();
            SetDrive(-50, -50, -4);
            wait(800, msec);
            indexer.open();
            wait(2500, msec);
            indexer.close();
            LeftSwing(135, 50, 10, 24);
            Shift(-2, 100, 20, 5, 1, 35, 100, 20, 24, 0.8);
            wait(1000,msec);
            intakeOut(0.6);
        }
        else if (autonNumber == 4)
        {
            Shift(10, 100, 20, 5, 1, 26, 100, 20, 18, 0.8);
            RightSwing(-90, 50, 10, 90);
            matchLoader.open();
            wait(500, msec);
            SetDrive(60, 60, 0);
            wait(1000, msec);
            intakeIn();
            Shift(1, 100, 20, 5, 1, -24, 100, 20, 18, 0.8);
            matchLoader.close();
            SetDrive(-50, -50, 4);
            wait(800, msec);
            indexer.open();
            wait(2500, msec);
            indexer.close();
            RightSwing(-135, 50, 10, 24);
            Shift(4, 100, 15, 5, 1, 34, 100, 20, 24, 0.8);
            Turn(180, 30, 10, 90);
            Shift(0, 100, 15, 5, 1, -2, 100, 20, 24, 0.8);
            indexer.open();
            wait(2000,msec);
            SetDrive(-30,-30);
        }
        else if (autonNumber == 5)
        {
            Shift(-10, 100, 20, 5, 1, 26, 100, 20, 18, 0.8);
            LeftSwing(90, 50, 10, 90);
            matchLoader.open();
            wait(500, msec);
            SetDrive(60, 60, 0);
            wait(1000, msec);
            Shift(-1, 100, 20, 5, 1, -24, 100, 20, 18, 0.8);
            SetDrive(-50, -50, -4);
            wait(800, msec);
            intakeIn();
            indexer.open();
        }
        else if (autonNumber == 6)
        {
            Shift(10, 100, 20, 5, 1, 26, 100, 20, 18, 0.8);
            RightSwing(-90, 50, 10, 90);
            matchLoader.open();
            wait(500, msec);
            SetDrive(60, 60, 0);
            wait(1000, msec);
            Shift(1, 100, 20, 5, 1, -24, 100, 20, 18, 0.8);
            SetDrive(-50, -50, 4);
            wait(800, msec);
            intakeIn();
            indexer.open();
        }
        else if (autonNumber == 7)
        {
        }
        else if (autonNumber == 8)
        {
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