#include "utils.h"
#include "dutils.h"
#include "log.h"
#include "pow.h"

int int_abs(const int num)
{
    if (num < 0)
    {
        return -num;
    }
    return num;
}

long long_abs(const long num)
{
    if (num < 0)
    {
        return -num;
    }
    return num;
}

long long long_long_abs(const long long num)
{
    if (num < 0)
    {
        return -num;
    }
    return num;
}

short long_long_digits(const long long num)
{
    const long long absolute = long_abs(num);
    // No laughing, StackOverflow says that this method is fast
  // clang-format off
  if (absolute < 10) { return 1; }
  if (absolute < 100) { return 2; }
  if (absolute < 1000) { return 3; }
  if (absolute < 10000) { return 4; }
  if (absolute < 100000) { return 5; }
  if (absolute < 1000000) { return 6; }
  if (absolute < 10000000) { return 7; }
  if (absolute < 100000000) { return 8; }
  if (absolute < 1000000000) { return 9; }
  if (absolute < 10000000000) { return 10; }
  if (absolute < 100000000000) { return 11; }
  if (absolute < 1000000000000) { return 12; }
  if (absolute < 10000000000000) { return 13; }
  if (absolute < 100000000000000) { return 14; }
  if (absolute < 1000000000000000) { return 15; }
  if (absolute < 10000000000000000) { return 16; }
  if (absolute < 100000000000000000) { return 17; }
  if (absolute < 1000000000000000000) { return 18; }
    // clang-format on
    return 19;
}

short double_digits_whole(const double num) { return (short)log_n(num, 10) + 1; }

static short check_places(double num, short places)
{
    // short i = 0;
    // struct ImproperFraction f = double_to_fraction(num - dfloor(num));
    // printf("%lli\n", f.numerator);
    // while (i < places) {
    //   double n = num * pow_di(10, i);
    //   // printf("%f, ", dmodulo(dround(n), 10));
    //   if (dmodulo(dround(n), 10) != 0) {
    //     // printf("\n");
    //     return 1;
    //   }

    //   i++;
    // }
    // // printf("\n");
    // return 0;
    return 1;
}

// TODO: figure out what the heck is going on here
short double_digits_partial(const double num)
{
    short digits = 0;
    while (dmodulo(num * pow_di(10, digits), 1.0) != 0 &&
        check_places(num * pow_di(10, digits), 3))
    {
        digits++;
    }

    return digits;
}

double deg_to_rad(const double degrees) { return PI * degrees / 180; }

int imaximum(const int x, const int y)
{
    if (x > y)
    {
        return x;
    }
    return y;
}

long lmaximum(const long x, const long y)
{
    if (x > y)
    {
        return x;
    }
    return y;
}

double round_to_threshold(const double num)
{
    const double rounded = dround(num);
    if (double_abs(rounded - num) <= THRESHOLD)
    {
        return rounded;
    }
    return num;
}
