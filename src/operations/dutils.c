#include "dutils.h"

double ddouble(double num, const int n)
{
    double_cast cast = (double_cast)num;
    int exp = cast.parts.exponent;
    exp += n;
    cast.parts.exponent = exp;
    return cast.d;
}

double dhalve(double num, const int n)
{
    auto cast = (double_cast)num;
    int exp = cast.parts.exponent;
    exp -= n;
    cast.parts.exponent = exp;
    return cast.d;
}

double double_abs(const double num)
{
    if (num < 0.0)
    {
        return -num;
    }
    return num;
}

double dfloor(double num)
{
    long long round = (long long)num;
    if (num < 0.0)
    {
        round -= 1;
    }
    return (double)round;
}

double dceil(const double num) { return -dfloor(-num); }

double dround(const double num)
{
    const double abs = double_abs(num);
    const double sign = num / abs;
    if (dmodulo(abs, 1.0) < 0.5)
    {
        return dfloor(abs) * sign;
    }
    return dceil(abs) * sign;
}

double dmodulo(const double num, const double mod)
{
    const double div = dfloor(double_abs(num / mod));
    const double out = num - mod * div;
    if (num < 0)
    {
        return -out;
    }
    return out;
}
