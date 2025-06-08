#include "Ports.h"

controller Controller = controller(primary);
competition Competition = competition();

motor left1 = motor(PORT18, ratio6_1, true);
motor left2 = motor(PORT15, ratio6_1, true);
motor left3 = motor(PORT16, ratio6_1, false);
motor_group leftMotors = motor_group(left1, left2, left3);
motor right1 = motor(PORT17, ratio6_1, false);
motor right2 = motor(PORT2, ratio6_1, false);
motor right3 = motor(PORT1, ratio6_1, true);
motor_group rightMotors = motor_group(right1, right2, right3);
motor strafe1 = motor(PORT20, ratio6_1, true);
motor_group strafe = motor_group(strafe1);
motor intake = motor(PORT19, ratio6_1, true);