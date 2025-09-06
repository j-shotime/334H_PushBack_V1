#define PERCENT_TO_MV 120
#define BLUE ratio6_1
#define GREEN ratio18_1
#define RED ratio36_1
double PercentToMillivolts(double percent);

bool withinRect(double X, double Y, double rectX, double rectY, double width, double height);

enum class autonType : int
{
    skills,
    red,
    blue
};