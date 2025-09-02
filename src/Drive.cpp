#include "everything.h"

motor_group motors[4];

inertial Inertial(0);

DriveTypes DriveType;

double leftSpeed;
double rightSpeed;
double strafeSpeed;

double forwardRatio;
double strafeRatio;

double targetAngle;

brakeType currentBrakeType = brakeType::coast;

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
    Set(FRONT_LEFT, left + strafe);
    Set(BACK_LEFT, left - strafe);
    Set(FRONT_RIGHT, right - strafe);
    Set(BACK_RIGHT, right + strafe);
}

#pragma endregion SetDrives

void SetupTankDrive(int InertialPort, double inputGear, double outputGear, double wheelDiameter, motor_group &Left, motor_group &Right)
{
    DriveType = DriveTypes::TankDrive;
    Inertial = inertial(InertialPort);
    forwardRatio = (inputGear / outputGear) * wheelDiameter * M_PI;
    motors[LEFT] = Left;
    motors[RIGHT] = Right;
    Inertial.resetRotation();
}

void SetupHDrive(int InertialPort, double inputGear, double outputGear, double wheelDiameter, double strafeInputGear, double strafeOutputGear, double strafeWheelDiameter, motor_group &Left, motor_group &Right, motor_group &Strafe)
{
    DriveType = DriveTypes::HDrive;
    Inertial = inertial(InertialPort);
    forwardRatio = (inputGear / outputGear) * wheelDiameter * M_PI;
    strafeRatio = (strafeInputGear / strafeOutputGear) * strafeWheelDiameter * M_PI;
    motors[LEFT] = Left;
    motors[RIGHT] = Right;
    motors[STRAFE] = Strafe;
    
    Inertial.resetRotation();
}

void SetupXDrive(int InertialPort, double inputGear, double outputGear, double wheelDiameter, motor_group &FrontLeft, motor_group &BackLeft, motor_group &FrontRight, motor_group &BackRight)
{
    DriveType = DriveTypes::XDrive;
    Inertial = inertial(InertialPort);
    forwardRatio = (inputGear / outputGear) * M_SQRT2 * wheelDiameter * M_PI;
    motors[FRONT_LEFT] = FrontLeft;
    motors[BACK_LEFT] = BackLeft;
    motors[FRONT_RIGHT] = FrontRight;
    motors[BACK_RIGHT] = BackRight;
    
    Inertial.resetRotation();
}

double GetForwardDistance()
{
    switch (DriveType)
    {
    case DriveTypes::TankDrive:
        return (motors[LEFT].position(rev) + motors[RIGHT].position(rev)) * forwardRatio / 2;
        break;

    case DriveTypes::HDrive:
        return (motors[LEFT].position(rev) + motors[RIGHT].position(rev)) * forwardRatio / 2;
        break;

    case DriveTypes::XDrive:
        return (motors[FRONT_LEFT].position(rev) + motors[BACK_LEFT].position(rev) + motors[FRONT_RIGHT].position(rev) + motors[BACK_RIGHT].position(rev)) * forwardRatio / 4;
        break;
    }
}

double GetForwardSpeed()
{
    switch (DriveType)
    {
    case DriveTypes::TankDrive:
        return (motors[LEFT].velocity(percent) + motors[RIGHT].velocity(percent)) / 2;
        break;

    case DriveTypes::HDrive:
        return (motors[LEFT].velocity(percent) + motors[RIGHT].velocity(percent)) / 2;
        break;

    case DriveTypes::XDrive:
        return (motors[FRONT_LEFT].velocity(percent) + motors[BACK_LEFT].velocity(percent) + motors[FRONT_RIGHT].velocity(percent) + motors[BACK_RIGHT].velocity(percent)) / 4;
        break;
    }
}

double GetStrafeDistance()
{
    switch (DriveType)
    {
    case DriveTypes::TankDrive:
        return 0;
        break;

    case DriveTypes::HDrive:
        return motors[STRAFE].position(rev) * strafeRatio;
        break;

    case DriveTypes::XDrive:
        return (motors[FRONT_LEFT].position(rev) - motors[BACK_LEFT].position(rev) + motors[FRONT_RIGHT].position(rev) - motors[BACK_RIGHT].position(rev)) * strafeRatio / 4;
        break;
    }
}

double GetStrafeSpeed()
{
    switch (DriveType)
    {
    case DriveTypes::TankDrive:
        return 0;
        break;

    case DriveTypes::HDrive:
        return motors[STRAFE].velocity(percent);
        break;

    case DriveTypes::XDrive:
        return (motors[FRONT_LEFT].velocity(percent) - motors[BACK_LEFT].velocity(percent) + motors[FRONT_RIGHT].velocity(percent) - motors[BACK_RIGHT].velocity(percent)) / 4;
        break;
    }
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
    SetDrive(leftSpeed, rightSpeed, power);
}

double RAJ(double distanceFromTarget, double kP)
{
    return distanceFromTarget * kP;
}

double motorRAJ(double distanceFromTarget, double maxSpeed, double Velocity, double decelerateDistance, double acceleration)
{
    return weightedAverage(Clip(RAJ(distanceFromTarget, 1/decelerateDistance), -maxSpeed/100, maxSpeed/100), Velocity/100, acceleration)*100;
}

void Shift(double Strafe, double MaxStrafe, double MinStrafeSpeed, double StrafeDecelerationDistance, double StrafeAcceleration, double Forward, double MaxForward, double MinForwardSpeed, double ForwardDecelerationDistance, double ForwardAcceleration)
{
    motors[LEFT].resetPosition();
    motors[RIGHT].resetPosition();
    motors[STRAFE].resetPosition();
    double StrafeError = Strafe - GetStrafeDistance();
    double ForwardError = Forward - GetForwardDistance();
    double strafeSpeed = motorRAJ(StrafeError, MaxStrafe, GetStrafeSpeed(), StrafeDecelerationDistance, StrafeAcceleration);
    double forwardSpeed = motorRAJ(ForwardError, MaxForward, GetForwardSpeed(), ForwardDecelerationDistance, ForwardAcceleration);
    strafeSpeed = MinSpeed(strafeSpeed, MinStrafeSpeed);
    forwardSpeed = MinSpeed(forwardSpeed, MinForwardSpeed);

    while (abs(StrafeError) > 1 || abs(ForwardError) > 1)
    {
        StrafeError = Strafe - GetStrafeDistance();
        ForwardError = Forward - GetForwardDistance();
        Controller.Screen.newLine();
        Controller.Screen.print("%f", Inertial.rotation(rotationUnits::deg));
        // Implement the control logic here
        strafeSpeed = motorRAJ(StrafeError, MaxStrafe, GetStrafeSpeed(), StrafeDecelerationDistance, StrafeAcceleration);
        forwardSpeed = motorRAJ(ForwardError, MaxForward, GetForwardSpeed(), ForwardDecelerationDistance, ForwardAcceleration);
        strafeSpeed = MinSpeed(strafeSpeed, MinStrafeSpeed);
        forwardSpeed = MinSpeed(forwardSpeed, MinForwardSpeed);
        SetDrive(forwardSpeed-(Inertial.rotation(rotationUnits::deg)-targetAngle)*M_SQRT2, forwardSpeed+(Inertial.rotation(rotationUnits::deg)-targetAngle)*M_SQRT2, strafeSpeed);
    }
    SetDrive(0, 0, 0);
}

void Pivot(double angle, double MaxSpeed, double minSpeed,double decelerateAngle)
{
    targetAngle += angle;
    while (abs(Inertial.rotation(rotationUnits::deg)-targetAngle) > 5)
    {
        double speed = MinSpeed(Clip(RAJ(Inertial.rotation(rotationUnits::deg)-targetAngle, 1/decelerateAngle)*100, -MaxSpeed, MaxSpeed), minSpeed);
        Controller.Screen.newLine();
        Controller.Screen.print("%f", speed);
        SetDrive(-speed, speed, 0);
    }
    SetDrive(0, 0, 0);
}

void LeftSwing(double angle, double MaxSpeed, double minSpeed,double decelerateAngle)
{
    targetAngle += angle;
    while (abs(Inertial.rotation(rotationUnits::deg)-targetAngle) > 5)
    {
        double speed = MinSpeed(Clip(RAJ(Inertial.rotation(rotationUnits::deg)-targetAngle, 1/decelerateAngle)*100, -MaxSpeed, MaxSpeed), minSpeed);
        Controller.Screen.newLine();
        Controller.Screen.print("%f", speed);
        SetDrive(-speed, 0, 0);
    }
    SetDrive(0, 0, 0);
}

void RightSwing(double angle, double MaxSpeed, double minSpeed,double decelerateAngle)
{
    targetAngle += angle;
    while (abs(Inertial.rotation(rotationUnits::deg)-targetAngle) > 5)
    {
        double speed = MinSpeed(Clip(RAJ(Inertial.rotation(rotationUnits::deg)-targetAngle, 1/decelerateAngle)*100, -MaxSpeed, MaxSpeed), minSpeed);
        Controller.Screen.newLine();
        Controller.Screen.print("%f", speed);
        SetDrive(0, speed, 0);
    }
    SetDrive(0, 0, 0);
}

void StartShift(double Strafe, double MaxStrafe, double MinStrafeSpeed, double StrafeDecelerationDistance, double StrafeAcceleration, double StrafeStop, double Forward, double MaxForward, double MinForwardSpeed, double ForwardDecelerationDistance, double ForwardAcceleration, double ForwardStop)
{
    // Implement the shifting logic using the provided parameters
}

void MidShift(double Strafe, double MaxStrafe, double MinStrafeSpeed, double StrafeDecelerationDistance, double StrafeAcceleration, double Forward, double MaxForward, double MinForwardSpeed, double ForwardDecelerationDistance, double ForwardAcceleration)
{
    // Implement the shifting logic using the provided parameters
}

void EndShift(double Strafe, double MaxStrafe, double MinStrafeSpeed, double StrafeDecelerationDistance, double StrafeAcceleration, double Forward, double MaxForward, double MinForwardSpeed, double ForwardDecelerationDistance, double ForwardAcceleration)
{
    // Implement the shifting logic using the provided parameters
}

double Clip(double value, double min, double max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

double MinSpeed(double speed, double minSpeed)
{
    if (abs(speed) < minSpeed)
    {
        return (speed < 0 ? -minSpeed : minSpeed);
    }
    return speed;
}

double weightedAverage(double a, double b, double weight)
{
    return (a * weight + b * (1 - weight));
}

void Brake()
{
    for (int i = 0; i < 4; i++)
    {
        motors[i].stop(currentBrakeType);
    }
}

void Brake(brakeType mode)
{
    currentBrakeType = mode;
    Brake();
}