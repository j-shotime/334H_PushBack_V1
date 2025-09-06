#include "Utilities.h"

double PercentToMillivolts(double percent)
{
    return percent * PERCENT_TO_MV;
}

bool withinRect(double X, double Y, double rectX, double rectY, double width, double height)
{
    return X>rectX && X < rectX+width && Y>rectY && Y < rectY+height;
}