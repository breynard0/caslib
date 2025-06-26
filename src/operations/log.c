#include "../../include/dutils.h"
#include "../../include/flags.h"
#include "../../include/fraction.h"
#include "../../include/pow.h"
#include "../../include/utils.h"

#define LN2 0.6931471805599453094172321214581765680755

double ln(double x) {
  double y = (x - 1) / (x + 1);
  double sum = 0;
  int iterations = 30 * x;

  for (int i = 0; i < iterations; i++) {
    double z = ddouble((double)i, 1) + 1.0;
    sum += (1.0 / z) * pow_di(y, z);
  }

  return 2.0 * sum;
}

double log_2(double num) {
  double_cast cast = (double_cast)num;
  int exponent = cast.parts.exponent - 1023;
  double remainder = num / pow_di(2.0, exponent);
  return exponent + (ln(remainder) / LN2);
}

double log_n(double num, double base) { return log_2(num) / log_2(base); }
