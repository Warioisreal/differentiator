#include <math.h>

#include "math_func.h"

double Add(double a, double b) { return a + b; }
double Sub(double a, double b) { return a - b; }
double Mul(double a, double b) { return a * b; }
double Div(double a, double b) { return a / b; }

//----------------------------------------------------------------------------------

double Sin(double a)  { return sin(a * M_PI / 180); }
double Cos(double a)  { return cos(a * M_PI / 180); }
double Tan(double a)  { return tan(a * M_PI / 180); }
double Ctan(double a) { return 1 / tan(a * M_PI / 180); }

//----------------------------------------------------------------------------------

double Arcsin(double a)  { return asin(a) * 180 / M_PI; }
double Arccos(double a)  { return acos(a) * 180 / M_PI; }
double Arctan(double a)  { return atan(a) * 180 / M_PI; }
double Arcctan(double a) { return (M_PI / 2 - atan(a)) * 180 / M_PI; }

//----------------------------------------------------------------------------------

double Sinh(double a)  { return sinh(a); }
double Cosh(double a)  { return cosh(a); }
double Tanh(double a)  { return tanh(a); }
double Ctanh(double a) { return 1 / tanh(a); }

//----------------------------------------------------------------------------------

double Arcsinh(double a)  { return asinh(a); }
double Arccosh(double a)  { return acosh(a); }
double Arctanh(double a)  { return atanh(a); }
double Arcctanh(double a) { return M_PI / 2 - atan(a * 180 / M_PI); }

//----------------------------------------------------------------------------------

double Log(double a, double b) { return log(a) / log(b); }

//----------------------------------------------------------------------------------

double Deg(double a, double b) { return pow(a, b); }

//----------------------------------------------------------------------------------

double GetE(void)  { return M_E; }
double GetPi(void) { return M_PI; }
