#include "../../include/dutils.h"

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

double double_abs(double num) {
  if (num < 0.0) {
    return -num;
  } else {
    return num;
  }
}

double dceil(double num) {
  return -dfloor(-num);
}

double dround(double num) {
  if (dmodulo(num, 1.0) < 0.5) {
    return dfloor(num);
  } else {
    return dceil(num);
  }
}

double dfloor(double num) {
  long round = (long)num;
  if (num < 0.0) {
    round -= 1;
  }
  return (double)round;
}

double dmodulo(double num, double mod) {
  double div = dfloor(double_abs(num / mod));
  double out = (num - (mod * div));
  if (num < 0) {
    return -out;
  } else {
    return out;
  }
}
