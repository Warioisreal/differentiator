#include <math.h>

#include "math_func.h"

double Add(double a, double b) {
    return a + b;
}

//----------------------------------------------------------------------------------

double Sub(double a, double b) {
    return a - b;
}

//----------------------------------------------------------------------------------

double Mul(double a, double b) {
    return a * b;
}

//----------------------------------------------------------------------------------

double Div(double a, double b) {
    return a / b;
}

//----------------------------------------------------------------------------------

double Sin(double a) {
    double rad_angle = M_PI * a / 180;
    return sin(rad_angle);
}

//----------------------------------------------------------------------------------

double Cos(double a) {
    double rad_angle = M_PI * a / 180;
    return cos(rad_angle);
}

//----------------------------------------------------------------------------------

double Tan(double a) {
    double rad_angle = M_PI * a / 180;
    return tan(rad_angle);
}

//----------------------------------------------------------------------------------

double Ctan(double a) {
    double rad_angle = M_PI * a / 180;
    return 1 / tan(rad_angle);
}
