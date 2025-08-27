#include "gcf.h"

#define PI 3.14159265359
#define NAT_LOG_BASE_E 2.718281828459
#define NAN 0.0 / 0.0
// #define THRESHOLD 0.000000001
#define THRESHOLD 0.01

int int_abs(int num);
long long_abs(long num);
long long long_long_abs(long long num);
short long_long_digits(long long num);
short double_digits_whole(double num);
short double_digits_partial(double num);
double deg_to_rad(double degrees);
long lmaximum(long x, long y);
int imaximum(int x, int y);
double round_to_threshold(double num);