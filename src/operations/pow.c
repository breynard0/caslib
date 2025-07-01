#include "../../auto-generated/powers_two.h"
#include "root.h"
#include "utils.h"
#include "dutils.h"
#include "log.h"
#include <stdio.h>

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
  if (exponent == 1) {
    return num;
  }

  int abs_exp = int_abs(exponent);
  double val = num;

  short lower_idx = 0;
  while (TWO_POWERS[lower_idx] <= abs_exp) {
    lower_idx++;
  }
  unsigned int lower = TWO_POWERS[lower_idx - 1];

  int cur_exp = 1;
  while (cur_exp < lower) {
    val *= val;
    cur_exp *= 2;
  }
  val *= pow_di(num, abs_exp - lower);

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
  double part = dmodulo(exponent, 1.0);
  int whole = exponent - part;
  
  int iterations = 100;
  
  double step = num / 4.0;
  double guess = num / 2.0;
  
  int i = 0;
  while (i < iterations) {
    if (log_n(guess, num) > part) {
      guess -= step;
    } else {
      guess += step;
    }
    step = dhalve(step, 1);
    i++;
  }
  
  return pow_di(num, whole) * guess;
}

double square_double(double num) { return pow_di(num, 2); }
double cube_double(double num) { return pow_di(num, 3); }
