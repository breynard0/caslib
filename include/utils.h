#include "gcf.h"

#define PI 3.14159265359

int int_abs(int num);
long long_abs(long num);
double double_abs(double num);
long pow_ll(long num, long exponent);
double pow_di(double num, int exponent);
double pow_frac(double num, struct ImproperFraction exponent);
double pow_dd(double num, double exponent);
short long_long_digits(long long num);
struct ImproperFraction double_to_fraction(double num);
struct MixedFraction double_to_mixed_fraction(double num);
double dfloor(double num);
double dmodulo(double num, double mod);
double deg_to_rad(double degrees);
long lmaximum(long x, long y);
