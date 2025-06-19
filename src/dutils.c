#include "../include/dutils.h"

double ddouble(double num, int n) {
    double_cast cast = (double_cast) num;
    int exp = cast.parts.exponent;
    exp += n;
    cast.parts.exponent = exp;
    return cast.d;
}

double dhalve(double num, int n) {
    double_cast cast = (double_cast) num;
    int exp = cast.parts.exponent;
    exp -= n;
    cast.parts.exponent = exp;
    return cast.d;
}
