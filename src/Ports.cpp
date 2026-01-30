#include "Ports.h"

controller Controller = controller(primary);
brain Brain = brain();
competition Competition = competition();
motor left1 = motor(PORT4, BLUE, true);
motor left2 = motor(PORT10, BLUE, true);
motor left3 = motor(PORT20, BLUE, true);
motor_group leftMotors = motor_group(left1, left2, left3);
motor right1 = motor(PORT17, BLUE, false);
motor right2 = motor(PORT18, BLUE, false);    
motor right3 = motor(PORT19, BLUE, false);
motor_group rightMotors = motor_group(right1, right2, right3);
motor strafe1 = motor(PORT1, GREEN, false);
motor_group strafe = motor_group(strafe1);
motor intake = motor(PORT11, BLUE, false);
pneumatics descore = pneumatics(Brain.ThreeWirePort.A);
pneumatics indexer = pneumatics(Brain.ThreeWirePort.G);
pneumatics matchLoader = pneumatics(Brain.ThreeWirePort.B);
inertial Inertial(PORT3);