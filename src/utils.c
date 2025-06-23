#include "../include/utils.h"
#include "../include/dutils.h"
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

long long_abs(long num) {
  if (num < 0) {
    return -num;
  } else {
    return num;
  }
}

long pow_ll(long num, long exponent) {
  if (exponent == 0) {
    return 1.0;
  }
  if (exponent < 0) {
    return 0.0;
  }
  int abs_exp = int_abs(exponent);
  long val = 1;
  for (int i = 0; i < abs_exp; i++) {
    val *= num;
  }

  return val;
}

long long pow_llll(long long num, long long exponent) {
  if (exponent == 0) {
    return 1.0;
  }
  if (exponent < 0) {
    return 0.0;
  }
  int abs_exp = int_abs(exponent);
  long long val = 1;
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
    short digit = (num - last) % i / pow_ll(10, count);
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

short long_long_digits(long long num) {
  long long absolute = long_abs(num);
  // No laughing, StackOverflow says that this method is fast
  // clang-format off
  if (num < 10) { return 1; }
  if (num < 100) { return 2; }
  if (num < 1000) { return 3; }
  if (num < 10000) { return 4; }
  if (num < 100000) { return 5; }
  if (num < 1000000) { return 6; }
  if (num < 10000000) { return 7; }
  if (num < 100000000) { return 8; }
  if (num < 1000000000) { return 9; }
  if (num < 10000000000) { return 10; }
  if (num < 100000000000) { return 11; }
  if (num < 1000000000000) { return 12; }
  if (num < 10000000000000) { return 13; }
  if (num < 100000000000000) { return 14; }
  if (num < 1000000000000000) { return 15; }
  if (num < 10000000000000000) { return 16; }
  if (num < 100000000000000000) { return 17; }
  if (num < 1000000000000000000) { return 18; }
  // clang-format on
  return 19;
}

static long long fix_long_long(long long num) {
  printf("num: %lli\n", num);
  short flip = 0;
  {
    unsigned long long i = 10;
    long long last = 0;
    int count = 0;
    int c = 0;

    while (i <= num) {
      long thing = pow_ll(10, count);
      short digit = ((num - last) % i) / pow_ll(10, count);
      if (digit == last) {
        c++;
      } else {
        c = 0;
      }

      if (c >= 5) {
        flip = 1;
      }

      last = digit;
      i *= 10;
      count++;
    }
  }

  if (flip) {
    long long new_num = 1;
    long long i = 10;
    long long last = 0;
    int last_digit = 0;
    int count = 0;
    int digits = long_long_digits(num);
    short good = 1;

    while (count < digits) {
      short digit = ((num - last) % i) / pow_ll(10, count);
      if ((digit >= last_digit ||
           ((digit >= (last_digit - 1)) && (count < 4))) &&
          good) {
        new_num *= 10;
      } else {
        new_num += (digit * pow_ll(10, count));
        good = 0;
      }

      last = num % i;
      last_digit = digit;
      i *= 10;
      count++;
    }

    return new_num;
  }

  return num;
}

const double MULTIPLIER = 1000000000000000000.0;
struct ImproperFraction double_to_fraction(double num) {
  short negative_mul = 2*(num >= 0)-1;
  num = double_abs(num);
  
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

  f.numerator = (long long)(double)f.numerator;
  f.numerator = fix_long_long(f.numerator);
  reduce_fraction(&f);
  f.numerator *= negative_mul;

  return f;
}

struct MixedFraction double_to_mixed_fraction(double num) {
  short is_negative = num <= 0;
  num = double_abs(num);
  
  double whole = dfloor(num);
  double decimal = num - whole;
  struct ImproperFraction i = double_to_fraction(decimal);
  struct MixedFraction out;
  out.numerator = i.numerator;
  out.denominator = i.denominator;
  out.integer = (long)whole;
  
  // Fix if negative
  if (is_negative) {
    out.numerator = -out.numerator;
    out.integer = -out.integer;
  }
  
  return out;
}

double deg_to_rad(double degrees) { return PI * degrees / 180; }

long lmaximum(long x, long y) {
  if (x > y) {
    return x;
  } else {
    return y;
  }
}
