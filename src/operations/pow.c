#include "../../auto-generated/powers_two.h"
#include "dutils.h"
#include "log.h"
#include "root.h"
#include "utils.h"

long pow_ll(const long num, const long exponent)
{
  if (exponent == 0) {
    return 1.0;
  }
  if (exponent < 0) {
    return 0.0;
  }
  const int abs_exp = int_abs(exponent);
  long val = 1;
  for (int i = 0; i < abs_exp; i++) {
    val *= num;
  }

  return val;
}

long long pow_llll(const long long num, const long long exponent)
{
  if (exponent == 0) {
    return 1.0;
  }
  if (exponent < 0) {
    return 0.0;
  }
  const int abs_exp = int_abs(exponent);
  long long val = 1;
  for (int i = 0; i < abs_exp; i++) {
    val *= num;
  }

  return val;
}

double pow_di(const double num, int exponent)
{
  if (double_abs(num) < THRESHOLD) {
    if (double_abs(exponent) > THRESHOLD) {
      return 0;
    }
    return 0.0 / 0.0;
  }

  if (exponent == 0) {
    return 1.0;
  }
  if (exponent == 1) {
    return num;
  }

  const int abs_exp = int_abs(exponent);
  double val = num;

  short lower_idx = 0;
  while (TWO_POWERS[lower_idx] <= abs_exp) {
    lower_idx++;
  }
  const unsigned int lower = TWO_POWERS[lower_idx - 1];

  int cur_exp = 1;
  while (cur_exp < lower) {
    val *= val;
    cur_exp *= 2;
  }
  val *= pow_di(num, abs_exp - lower);

  if (exponent > 0) {
    return val;
  }
  return 1.0 / val;
}

double pow_frac(const double num, const struct ImproperFraction exponent)
{
  struct ImproperFraction reduced = exponent;
  reduce_fraction(&reduced);
  return pow_di(nth_root(reduced.denominator, num), reduced.numerator);
}

double pow_dd(const double num, const double exponent)
{
  if (exponent == (long long)exponent) {
    return pow_di(num, exponent);
  }

  const double part = dmodulo(exponent, 1.0);
  const int whole = exponent - part;

  constexpr int iterations = 100;

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

double square_double(const double num) { return pow_di(num, 2); }
double cube_double(const double num) { return pow_di(num, 3); }
