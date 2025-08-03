#include "dutils.h"

double ddouble(double num, int n) {
  double_cast cast = (double_cast)num;
  int exp = cast.parts.exponent;
  exp += n;
  cast.parts.exponent = exp;
  return cast.d;
}

double dhalve(double num, int n) {
  double_cast cast = (double_cast)num;
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

double dceil(double num) { return -dfloor(-num); }

double dround(double num) {
  double abs = double_abs(num);
  double sign = num / abs;
  if (dmodulo(abs, 1.0) < 0.5) {
    return dfloor(abs) * sign;
  } else {
    return dceil(abs) * sign;
  }
}

double dfloor(double num) {
  long long round = (long long)num;
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
