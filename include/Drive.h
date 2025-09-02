enum DriveTypes
{
    TankDrive,
    XDrive,
    HDrive
};

#define FRONT_LEFT 0
#define BACK_LEFT 1
#define FRONT_RIGHT 2
#define BACK_RIGHT 3

#define LEFT 0
#define RIGHT 1

#define STRAFE 2

#define PERCENT_TO_MV 120

void SetupTankDrive(int InertialPort, double inputGear, double outputGear, double wheelDiameter, motor_group &Left, motor_group &Right);

void SetupHDrive(int InertialPort, double inputGear, double outputGear, double wheelDiameter, double strafeInputGear, double strafeOutputGear, double strafeWheelDiameter, motor_group &Left, motor_group &Right, motor_group &Strafe);

void SetupXDrive(int InertialPort, double inputGear, double outputGear, double wheelDiameter, motor_group &FrontLeft, motor_group &BackLeft, motor_group &FrontRight, motor_group &BackRight);

void SetDrive(double left, double right, double strafe);

void SetDrive(double left, double right);

void SetValues(double left, double right, double strafe);

void SetLeft(double power);

void SetRight(double power);

void SetStrafe(double power);

void Shift(double Strafe, double MaxStrafe, double MinStrafeSpeed, double StrafeDecelerationDistance, double StrafeAcceleration, double Forward, double MaxForward, double MinForwardSpeed, double ForwardDecelerationDistance, double ForwardAcceleration);

double GetForwardDistance();

double GetStrafeDistance();

double Clip(double value, double min, double max);

double MinSpeed(double speed, double min);

double weightedAverage(double value1, double value2, double weight);

void Pivot(double angle, double MaxSpeed, double minSpeed, double decelerateAngle);

void RightSwing(double angle, double MaxSpeed, double minSpeed, double decelerateAngle);

void LeftSwing(double angle, double MaxSpeed, double minSpeed, double decelerateAngle);

void Brake();

void Brake(brakeType mode);