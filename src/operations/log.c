#include "dutils.h"
#include "pow.h"

#define LN2 0.6931471805599453094172321214581765680755

double ln(const double x)
{
    const double y = (x - 1) / (x + 1);
    double sum = 0;
    const int iterations = 30 * x;

    for (int i = 0; i < iterations; i++)
    {
        const double z = ddouble(i, 1) + 1.0;
        sum += 1.0 / z * pow_di(y, z);
    }

    return 2.0 * sum;
}

double log_2(double num)
{
    const auto cast = (double_cast)num;
    const int exponent = cast.parts.exponent - 1023;
    const double remainder = num / pow_di(2.0, exponent);
    return exponent + ln(remainder) / LN2;
}

double log_n(const double num, const double base) { return log_2(num) / log_2(base); }
