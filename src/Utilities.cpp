#include "Utilities.h"

inline double PercentToMillivolts(double percent)
{
    return percent * PERCENT_TO_MV;
}