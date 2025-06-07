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

void SetupTankDrive(int InertialPort, motor_group &Left, motor_group &Right);

void SetupHDrive(int InertialPort, motor_group &Left, motor_group &Right, motor_group &Strafe);

void SetupXDrive(int InertialPort, motor_group &FrontLeft, motor_group &BackLeft, motor_group &FrontRight, motor_group &BackRight);

void SetDrive(double left, double right, double strafe);

void SetDrive(double left, double right);

void SetValues(double left, double right, double strafe);

void SetLeft(double power);

void SetRight(double power);

void SetStrafe(double power);