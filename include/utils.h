#include "gcf.h"

#define PI 3.14159265359

int int_abs(int num);
long long_abs(long num);
long pow_ll(long num, long exponent);
long long pow_llll(long long num, long long exponent);
double pow_di(double num, int exponent);
double pow_frac(double num, struct ImproperFraction exponent);
double pow_dd(double num, double exponent);
short long_long_digits(long long num);
struct ImproperFraction double_to_fraction(double num);
struct MixedFraction double_to_mixed_fraction(double num);
double deg_to_rad(double degrees);
long lmaximum(long x, long y);
