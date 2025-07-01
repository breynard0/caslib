#include "dutils.h"
#include "pow.h"
#include "utils.h"

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

static long long fix_long_long(long long num) {
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
  short negative_mul = 2 * (num >= 0) - 1;
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
