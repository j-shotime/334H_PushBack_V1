#include "everything.h"

controller Controller;

motor left1 = motor(PORT11, ratio6_1, true);
motor left2 = motor(PORT13, ratio6_1, false);
motor left3 = motor(PORT1, ratio6_1, true);
motor_group leftMotors = motor_group(left1, left2, left3);
motor right1 = motor(PORT12, ratio6_1, false);
motor right2 = motor(PORT14, ratio6_1, true);
motor right3 = motor(PORT2, ratio6_1, false);
motor_group rightMotors = motor_group(right1, right2, right3);
motor strafe1 = motor(PORT20, ratio6_1, true);
motor_group strafe = motor_group(strafe1);
motor intake = motor(PORT19, ratio6_1, true);