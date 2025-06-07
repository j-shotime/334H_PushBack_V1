#include "everything.h"
#include "Utilities.h"

motor_group motors[4]; 

inertial Inertial(0);

DriveTypes DriveType;

double leftSpeed;
double rightSpeed;
double strafeSpeed;

void SetupTankDrive(int InertialPort, motor_group &Left, motor_group &Right)
{
    DriveType = DriveTypes::TankDrive;
    Inertial = inertial(InertialPort);
    motors[LEFT] = Left;
    motors[RIGHT] = Right;
}

void SetupHDrive(int InertialPort, motor_group &Left, motor_group &Right, motor_group &Strafe)
{
    DriveType = DriveTypes::HDrive;
    Inertial = inertial(InertialPort);
    motors[LEFT] = Left;
    motors[RIGHT] = Right;
    motors[STRAFE] = Strafe;
}

void SetupXDrive(int InertialPort, motor_group &FrontLeft, motor_group &BackLeft, motor_group &FrontRight, motor_group &BackRight)
{
    DriveType = DriveTypes::XDrive;
    Inertial = inertial(InertialPort);
    motors[FRONT_LEFT] = FrontLeft;
    motors[BACK_LEFT] = BackLeft;
    motors[FRONT_RIGHT] = FrontRight;
    motors[BACK_RIGHT] = BackRight;
}

void SetDrive(double left, double right, double strafe)
{
    SetValues(left, right, strafe);
    switch (DriveType)
    {
        case TankDrive:
        SetTankDrive(left, right);
        break;
        case HDrive:
        SetHDrive(left, right, strafe);
        break;
        case XDrive:
        SetXDrive(left, right, strafe);
        break;
    }
}

void SetDrive(double left, double right)
{
    SetDrive(left, right, strafeSpeed);
}

void SetLeft(double power)
{
    SetDrive(power, rightSpeed, strafeSpeed);
}

void SetRight(double power)
{
    SetDrive(leftSpeed, power, strafeSpeed);
}

void SetStrafe(double power)
{
    SetDrive(leftSpeed, rightSpeed ,power);
}

#pragma region Controls

void Set(int index, double power)
{
    motors[index].spin(fwd, PercentToMillivolts(power), voltageUnits::mV);
}

void SetValues(double left, double right, double strafe)
{
    leftSpeed = left;
    rightSpeed = right;
    strafeSpeed = strafe;
}

#pragma endregion Controls

#pragma region SetDrives

void SetTankDrive(double left, double right)
{
    Set(LEFT, left);
    Set(RIGHT, right);
}

void SetHDrive(double left, double right, double strafe)
{
    Set(LEFT, left);
    Set(RIGHT, right);
    Set(STRAFE, strafe);
}

void SetXDrive(double left, double right, double strafe)
{
    Set(FRONT_LEFT, left+strafe);
    Set(BACK_LEFT, left-strafe);
    Set(FRONT_RIGHT, right-strafe);
    Set(BACK_RIGHT, right+strafe);
}

#pragma endregion SetDrives