#include "atrig.h"
#include "dutils.h"
#include "pow.h"
#include "root.h"
#include "trig.h"
#include "utils.h"

#include "../../auto-generated/cordic_constants.h"

enum FuncType { fSINE, fCOSINE, fTANGENT };

static double arc_compute(double num, const enum FuncType type)
{
    short is_negative = 1;

    if (num < 0)
    {
        is_negative = -1;
    }

    num = double_abs(num);
    if (num > 1 && type != fTANGENT)
    {
        return NAN;
    }

    double angle_sum = 0.0;
    double x = 1.0;
    double y = 0.0;

    // Near 1, sine and cosine freak out
    // Tangent in this function doesn't work quite right at large values
    // Cosine values passed into arctan should be small, so that is not an issue
    // Sine values passed into tan may be large, so it is likely inaccurate, hence
    // the workaround The tangent function exported by this file does sinX/cosX to
    // avoid the issues in the arc_compute function
    const double threshold = 0.99;
    if (num > threshold && type != fTANGENT)
    {
        double cos_x = num;
        if (type == fSINE)
        {
            cos_x = square_root(1.0 - pow_di(num, 2));
            return arc_cosine(cos_x);
        }
        return arc_compute(
            (is_negative * (square_root(1.0 - pow_di(cos_x, 2)) / cos_x)),
            fTANGENT);
    }

    int i = 0;
    while (i < CORDIC_COUNT)
    {
        double val = 0;
        switch (type)
        {
        case fSINE:
            val = y;
            break;
        case fCOSINE:
            val = x;
            break;
        case fTANGENT:
            val = y / x;
            break;
        }

        const double x_last = x;
        const double y_last = y;
        const double angle = CORDIC_CONSTANTS[i];

        const double halved_x = dhalve(x, i);
        const double halved_y = dhalve(y, i);

        // Not using pre-computed values of cosine since Y is being directly
        // compared
        if (val < num)
        {
            x = (x_last - halved_y) * cosine(angle);
            y = (y_last + halved_x) * cosine(angle);
            angle_sum += angle;
        }
        else
        {
            x = (x_last + halved_y) * cosine(angle);
            y = (y_last - halved_x) * cosine(angle);
            angle_sum -= angle;
        }

        i++;
    }

    if (type == fCOSINE)
    {
        angle_sum = -angle_sum;
        if (is_negative == -1)
        {
            return PI - angle_sum;
        }
        else
        {
            return angle_sum;
        }
    }

    return angle_sum * is_negative;
}

double arc_sine(const double num) { return arc_compute(num, fSINE); }

double arc_cosine(const double num) { return arc_compute(num, fCOSINE); }

double arc_tangent(const double num)
{
    return arc_cosine(1 / square_root(1 + num * num));
}