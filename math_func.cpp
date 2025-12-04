#include <math.h>

#include "math_func.h"

double Add(double a, double b) { return a + b; }
double Sub(double a, double b) { return a - b; }
double Mul(double a, double b) { return a * b; }
double Div(double a, double b) { return a / b; }

//----------------------------------------------------------------------------------

double Sin(double a)  { return sin(a); }
double Cos(double a)  { return cos(a); }
double Tan(double a)  { return tan(a); }
double Ctan(double a) { return 1 / tan(a); }

//----------------------------------------------------------------------------------

double Arcsin(double a)  { return asin(a); }
double Arccos(double a)  { return acos(a); }
double Arctan(double a)  { return atan(a); }
double Arcctan(double a) { return M_PI / 2 - atan(a); }

//----------------------------------------------------------------------------------

double Sinh(double a)  { return sinh(a); }
double Cosh(double a)  { return cosh(a); }
double Tanh(double a)  { return tanh(a); }
double Ctanh(double a) { return 1 / tanh(a); }

//----------------------------------------------------------------------------------

double Arcsinh(double a)  { return asinh(a); }
double Arccosh(double a)  { return acosh(a); }
double Arctanh(double a)  { return atanh(a); }
double Arcctanh(double a) { return M_PI / 2 - atan(a); }

//----------------------------------------------------------------------------------

double Log(double a, double b) { return log(a) / log(b); }

//----------------------------------------------------------------------------------

double Pow(double a, double b) { return pow(a, b); }

//----------------------------------------------------------------------------------

double GetE(void)  { return M_E; }
double GetPi(void) { return M_PI; }

//----------------------------------------------------------------------------------

double Factorial(double a) {
    if (a <= 1) { return 1; }
    return a * Factorial(a - 1);
}
