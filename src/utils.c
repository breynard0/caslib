#include "../include/utils.h"
#include "../include/gcf.h"
#include "../include/root.h"
#include "../include/flags.h"
#include <stdio.h>

int int_abs(int num) {
  if (num < 0) {
    return -num;
  } else {
    return num;
  }
}

double double_abs(double num) {
  if (num < 0.0) {
    return -num;
  } else {
    return num;
  }
}

double pow_di(double num, int exponent) {
  if (exponent == 0) {
    return 1.0;
  }

  int abs_exp = int_abs(exponent);
  double val = 1.0;

  for (int i = 0; i < abs_exp; i++) {
    val *= num;
  }

  if (exponent > 0) {
    return val;
  } else {
    return 1.0/val;
  }
}

double pow_frac(double num, struct Fraction exponent) {
  struct Fraction reduced = exponent;
  reduce_fraction(&reduced);
  return pow_di(nth_root(reduced.denominator, num), reduced.numerator);
}

const double MULTIPLIER = 1000000000000000000.0;
struct Fraction double_to_fraction(double num) {
  double whole = dfloor(num);
  double decimal = num - whole;
  struct Fraction f;
  f.numerator = (long) (decimal * MULTIPLIER);
  f.denominator = (long) MULTIPLIER;
  reduce_fraction(&f);
  f.numerator += (long)whole * f.denominator;
  f_overflow |= ((num < 0.0) != (f.numerator < 0));
  return f;
}

double dfloor(double num) {
    long round = (long) num;
    if (num < 0.0) {
        round -= 1;
    }
    return (double) round;
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

double deg_to_rad(double degrees) {
    return PI * degrees / 180;
}

long lmaximum(long x, long y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}
