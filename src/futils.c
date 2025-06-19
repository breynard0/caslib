#include "../include/futils.h"

float fdouble(float num, int n) {
    float_cast cast = (float_cast) num;
    int exp = cast.parts.exponent;
    exp += n;
    cast.parts.exponent = exp;
    return cast.f;
}

float fhalve(float num, int n) {
    float_cast cast = (float_cast) num;
    int exp = cast.parts.exponent;
    exp -= n;
    cast.parts.exponent = exp;
    return cast.f;
}
