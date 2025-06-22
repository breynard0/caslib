#include "../include/utils.h"
#include "../include/flags.h"
#include "../include/gcf.h"
#include "../include/root.h"
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

double pow_ii(int num, int exponent) {
  if (exponent == 0) {
    return 1.0;
  }
  if (exponent < 0) {
    return 0.0;
  }
  int abs_exp = int_abs(num);
  int val = 1;
  for (int i = 0; i < abs_exp; i++) {
    val *= num;
  }
  
  return val;
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
    return 1.0 / val;
  }
}

double pow_frac(double num, struct ImproperFraction exponent) {
  struct ImproperFraction reduced = exponent;
  reduce_fraction(&reduced);
  return pow_di(nth_root(reduced.denominator, num), reduced.numerator);
}

double pow_dd(double num, double exponent) {
  struct MixedFraction f = double_to_mixed_fraction(exponent);
  printf("%lli\n", f.numerator);
  struct ImproperFraction i;
  i.numerator = f.numerator;
  i.denominator = f.denominator;
  return pow_frac(num, i);
}

static short is_imprecise(long long num) {
  unsigned long long i = 10;
  int c = 0;
  long long last = 0;
  int count = 0;
  while (i <= num) {
    printf("%lli %d\n", (num - last) % i, 10^count);
    if (num % i == last) {
      c++;
    } else {
      c = 0;
    }
    
    if (c >= 5) {
      return 1;
    }

    last = num % i;
    i *= 10;
    count++;
  }

  return 0;
}

const double MULTIPLIER = 1000000000000000000.0;
struct ImproperFraction double_to_fraction(double num) {
  double multiplier = MULTIPLIER;
  
  double whole = dfloor(num);
  double decimal = num - whole;
  struct ImproperFraction f;

  do {
    f.numerator = (long long)(decimal * multiplier);
    f.denominator = (long)multiplier;
    reduce_fraction(&f);
    f.numerator += (long)whole * f.denominator;
    multiplier /= 10;
  } while (((num < 0.0) != (f.numerator < 0)) || is_imprecise(f.numerator));
  
  printf("%0.12lli\n", f.numerator);
  f.numerator = (long long)(double) f.numerator;
  reduce_fraction(&f);
  
  return f;
}

struct MixedFraction double_to_mixed_fraction(double num) {
  double whole = dfloor(num);
  double decimal = num - whole;
  struct ImproperFraction i = double_to_fraction(decimal);
  struct MixedFraction out;
  out.numerator = i.numerator;
  out.denominator = i.denominator;
  out.integer = (long) whole;
  return out;
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

double deg_to_rad(double degrees) { return PI * degrees / 180; }

long lmaximum(long x, long y) {
  if (x > y) {
    return x;
  } else {
    return y;
  }
}
