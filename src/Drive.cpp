#include "everything.h"

// ===================
// Drive State Globals
// ===================

// Array of motor groups for drive control
motor_group motors[4];

// Inertial sensor for heading/rotation

// Enum to track current drive type (Tank, H, X)
DriveTypes DriveType;

// Variables to store current drive speeds
double leftSpeed;
double rightSpeed;
double strafeSpeed;

// Ratios for converting motor rotations to distance
double forwardRatio;
double strafeRatio;

// Target heading for turns and corrections
double targetAngle;

// Current brake mode for the drive
brakeType currentBrakeType = brakeType::coast;

#pragma region Utility

// Clip a value between min and max
double Clip(double value, double min, double max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

// Ensure speed is at least minSpeed (preserving sign)
double MinSpeed(double speed, double minSpeed)
{
    if (abs(speed) < minSpeed)
    {
        return (speed < 0 ? -minSpeed : minSpeed);
    }
    return speed;
}

double Rotation()
{
    return Inertial.rotation();
}

// Weighted average between two values
double weightedAverage(double a, double b, double weight)
{
    return (a * weight + b * (1 - weight));
}

// Simple proportional controller for distance
double RAJ(double distanceFromTarget, double kP)
{
    return distanceFromTarget * kP;
}

// Calculate motor speed using RAJ, clip, and apply minSpeed (no weighted average)
double motorRAJ(double distanceFromTarget, double maxSpeed, double Velocity, double decelerateDistance, double minSpeed)
{
    double speed = Clip(RAJ(distanceFromTarget, 1/decelerateDistance), -maxSpeed/100, maxSpeed/100) * 100;
    return MinSpeed(speed, minSpeed);
}

#pragma endregion Utility

#pragma region Controls

// Set a specific motor group to a power value (converted to mV)
void Set(int index, double power)
{
    motors[index].spin(fwd, PercentToMillivolts(power), voltageUnits::mV);
}

// Store the current drive values for left, right, and strafe
void SetValues(double left, double right, double strafe)
{
    leftSpeed = left;
    rightSpeed = right;
    strafeSpeed = strafe;
}

// Set only the left drive value
void SetLeft(double power)
{
    SetDrive(power, rightSpeed, strafeSpeed);
}

// Set only the right drive value
void SetRight(double power)
{
    SetDrive(leftSpeed, power, strafeSpeed);
}

// Set only the strafe drive value
void SetStrafe(double power)
{
    SetDrive(leftSpeed, rightSpeed, power);
}

#pragma endregion Controls

#pragma region DriveSetters

// Set left and right motors for tank drive
void SetTankDrive(double left, double right)
{
    Set(LEFT, left);
    Set(RIGHT, right);
}

// Set left, right, and strafe motors for H drive
void SetHDrive(double left, double right, double strafe)
{
    Set(LEFT, left);
    Set(RIGHT, right);
    Set(STRAFE, strafe);
}

// Set all four motors for X drive (holonomic)
void SetXDrive(double left, double right, double strafe)
{
    Set(FRONT_LEFT, left + strafe);
    Set(BACK_LEFT, left - strafe);
    Set(FRONT_RIGHT, right - strafe);
    Set(BACK_RIGHT, right + strafe);
}

// Set drive values for all drive types
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

// Overload: Set drive with only left and right (keep current strafe)
void SetDrive(double left, double right)
{
    SetDrive(left, right, strafeSpeed);
}

#pragma endregion DriveSetters

#pragma region DriveSetup

// Configure tank drive and initialize inertial sensor and ratios
void SetupTankDrive(int InertialPort, double inputGear, double outputGear, double wheelDiameter, motor_group &Left, motor_group &Right)
{
    DriveType = DriveTypes::TankDrive;
    Inertial = inertial(InertialPort);
    forwardRatio = (inputGear / outputGear) * wheelDiameter * M_PI;
    motors[LEFT] = Left;
    motors[RIGHT] = Right;
    Inertial.calibrate();
}

// Configure H drive and initialize inertial sensor and ratios
void SetupHDrive(int InertialPort, double inputGear, double outputGear, double wheelDiameter, double strafeInputGear, double strafeOutputGear, double strafeWheelDiameter, motor_group &Left, motor_group &Right, motor_group &Strafe)
{
    DriveType = DriveTypes::HDrive;
    Inertial = inertial(InertialPort);
    forwardRatio = (inputGear / outputGear) * wheelDiameter * M_PI;
    strafeRatio = (strafeInputGear / strafeOutputGear) * strafeWheelDiameter * M_PI;
    motors[LEFT] = Left;
    motors[RIGHT] = Right;
    motors[STRAFE] = Strafe;
    Inertial.calibrate();
}

// Configure X drive and initialize inertial sensor and ratios
void SetupXDrive(int InertialPort, double inputGear, double outputGear, double wheelDiameter, motor_group &FrontLeft, motor_group &BackLeft, motor_group &FrontRight, motor_group &BackRight)
{
    DriveType = DriveTypes::XDrive;
    Inertial = inertial(InertialPort);
    forwardRatio = (inputGear / outputGear) * M_SQRT2 * wheelDiameter * M_PI;
    motors[FRONT_LEFT] = FrontLeft;
    motors[BACK_LEFT] = BackLeft;
    motors[FRONT_RIGHT] = FrontRight;
    motors[BACK_RIGHT] = BackRight;
    Inertial.calibrate();
}

#pragma endregion DriveSetup

#pragma region DriveGetters

// Get the average forward distance traveled based on drive type
double GetForwardDistance()
{
    switch (DriveType)
    {
    case DriveTypes::TankDrive:
        return (motors[LEFT].position(rev) + motors[RIGHT].position(rev)) * forwardRatio / 2;
    case DriveTypes::HDrive:
        return (motors[LEFT].position(rev) + motors[RIGHT].position(rev)) * forwardRatio / 2;
    case DriveTypes::XDrive:
        return (motors[FRONT_LEFT].position(rev) + motors[BACK_LEFT].position(rev) + motors[FRONT_RIGHT].position(rev) + motors[BACK_RIGHT].position(rev)) * forwardRatio / 4;
    }
    return 0;
}

// Get the average forward speed based on drive type
double GetForwardSpeed()
{
    switch (DriveType)
    {
    case DriveTypes::TankDrive:
        return (motors[LEFT].velocity(percent) + motors[RIGHT].velocity(percent)) / 2;
    case DriveTypes::HDrive:
        return (motors[LEFT].velocity(percent) + motors[RIGHT].velocity(percent)) / 2;
    case DriveTypes::XDrive:
        return (motors[FRONT_LEFT].velocity(percent) + motors[BACK_LEFT].velocity(percent) + motors[FRONT_RIGHT].velocity(percent) + motors[BACK_RIGHT].velocity(percent)) / 4;
    }
    return 0;
}

// Get the strafe distance based on drive type
double GetStrafeDistance()
{
    switch (DriveType)
    {
    case DriveTypes::TankDrive:
        return 0;
    case DriveTypes::HDrive:
        return motors[STRAFE].position(rev) * strafeRatio;
    case DriveTypes::XDrive:
        return (motors[FRONT_LEFT].position(rev) - motors[BACK_LEFT].position(rev) + motors[FRONT_RIGHT].position(rev) - motors[BACK_RIGHT].position(rev)) * strafeRatio / 4;
    }
    return 0;
}

// Get the strafe speed based on drive type
double GetStrafeSpeed()
{
    switch (DriveType)
    {
    case DriveTypes::TankDrive:
        return 0;
    case DriveTypes::HDrive:
        return motors[STRAFE].velocity(percent);
    case DriveTypes::XDrive:
        return (motors[FRONT_LEFT].velocity(percent) - motors[BACK_LEFT].velocity(percent) + motors[FRONT_RIGHT].velocity(percent) - motors[BACK_RIGHT].velocity(percent)) / 4;
    }
    return 0;
}

#pragma endregion DriveGetters

#pragma region AutonomousMovement

// Main shifting function for moving in both strafe and forward directions
void Shift(double Strafe, double MaxStrafe, double MinStrafeSpeed, double StrafeDecelerationDistance, double StrafeAcceleration, double Forward, double MaxForward, double MinForwardSpeed, double ForwardDecelerationDistance, double ForwardAcceleration)
{
    // Reset motor positions for accurate distance tracking
    motors[LEFT].resetPosition();
    motors[RIGHT].resetPosition();
    motors[STRAFE].resetPosition();
    double StrafeError = Strafe - GetStrafeDistance();
    double ForwardError = Forward - GetForwardDistance();

    // Initial speed calculations
    double strafeSpeed = motorRAJ(StrafeError, MaxStrafe, GetStrafeSpeed(), StrafeDecelerationDistance, MinStrafeSpeed);
    double forwardSpeed = motorRAJ(ForwardError, MaxForward, GetForwardSpeed(), ForwardDecelerationDistance, MinForwardSpeed);
    double correction = 0;
    // Calculate angle for heading correction
    double angle = abs(atan2(abs(Strafe), abs(Forward)));
    if (angle < 1)
        angle = 1;

    // Apply weighted average after MinSpeed
    strafeSpeed = weightedAverage(strafeSpeed, GetStrafeSpeed(), StrafeAcceleration);
    forwardSpeed = weightedAverage(forwardSpeed, GetForwardSpeed(), ForwardAcceleration);
    int time=0;
    // Loop until both errors are within threshold
    while (abs(StrafeError) > 1 || abs(ForwardError) > 1)
    {
        StrafeError = Strafe - GetStrafeDistance();
        ForwardError = Forward - GetForwardDistance();
        Controller.Screen.newLine();
        Controller.Screen.print("%f", Inertial.rotation(rotationUnits::deg));

        strafeSpeed = motorRAJ(StrafeError, MaxStrafe, GetStrafeSpeed(), StrafeDecelerationDistance, MinStrafeSpeed);
        forwardSpeed = motorRAJ(ForwardError, MaxForward, GetForwardSpeed(), ForwardDecelerationDistance, MinForwardSpeed);

        // Apply weighted average after MinSpeed
        strafeSpeed = weightedAverage(strafeSpeed, GetStrafeSpeed(), StrafeAcceleration);
        forwardSpeed = weightedAverage(forwardSpeed, GetForwardSpeed(), ForwardAcceleration);

        // Apply heading correction and set drive
        correction = (Inertial.rotation(rotationUnits::deg) - targetAngle) * angle * 2;

        SetDrive(forwardSpeed-correction, forwardSpeed+correction, strafeSpeed);
        wait(1,msec);
        
        if(abs(GetStrafeSpeed()+GetForwardSpeed())<MinForwardSpeed+MinStrafeSpeed)time++;
        if(time>500) break;
    }
    SetDrive(0, 0, 0);
}

// Pivot the robot to a specific angle using inertial sensor
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

// Left swing turn using only left side
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

// Right swing turn using only right side
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

void Turn(double angle, double MaxSpeed, double minSpeed,double decelerateAngle)
{
    targetAngle += angle;
    while (abs(Inertial.rotation(rotationUnits::deg)-targetAngle) > 5)
    {
        double speed = MinSpeed(Clip(RAJ(Inertial.rotation(rotationUnits::deg)-targetAngle, 1/decelerateAngle)*100, -MaxSpeed, MaxSpeed), minSpeed);
        Controller.Screen.newLine();
        Controller.Screen.print("%f", abs(Inertial.rotation(rotationUnits::deg)-targetAngle));
        SetDrive(-speed, speed, 0);
    }
    SetDrive(0, 0, 0);
}
// Placeholder for start of a shift sequence (implement as needed)
void StartShift(double Strafe, double MaxStrafe, double MinStrafeSpeed, double StrafeDecelerationDistance, double StrafeAcceleration, double StrafeStop, double Forward, double MaxForward, double MinForwardSpeed, double ForwardDecelerationDistance, double ForwardAcceleration, double ForwardStop)
{
    // Implement the shifting logic using the provided parameters
}

// Placeholder for middle of a shift sequence (implement as needed)
void MidShift(double Strafe, double MaxStrafe, double MinStrafeSpeed, double StrafeDecelerationDistance, double StrafeAcceleration, double Forward, double MaxForward, double MinForwardSpeed, double ForwardDecelerationDistance, double ForwardAcceleration)
{
    // Implement the shifting logic using the provided parameters
}

// Placeholder for end of a shift sequence (implement as needed)
void EndShift(double Strafe, double MaxStrafe, double MinStrafeSpeed, double StrafeDecelerationDistance, double StrafeAcceleration, double Forward, double MaxForward, double MinForwardSpeed, double ForwardDecelerationDistance, double ForwardAcceleration)
{
    // Implement the shifting logic using the provided parameters
}

#pragma endregion AutonomousMovement

#pragma region Braking

// Stop all motors using the current brake type
void Brake()
{
    for (int i = 0; i < 4; i++)
    {
        motors[i].stop(currentBrakeType);
    }
}

// Set the brake mode and stop all motors
void Brake(brakeType mode)
{
    currentBrakeType = mode;
    Brake();
}

#pragma endregion