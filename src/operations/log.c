#include "../../include/flags.h"
#include "../../include/fraction.h"
#include "../../include/pow.h"
#include "../../include/utils.h"
#include "../../include/dutils.h"
#include <stdio.h>

double log_n(double num, double base) {
  if (num <= 0.0) {
    if (num == 0.0) {
      f_undefined = 1;
    }
    return 0.0 / 0.0;
  }
  int lower = 0;
  double count = num;
  if (num >= 1.0) {
    while (count >= base) {
      count /= base;
      lower++;
    }
  } else {
    while (count < 1.0) {
      count *= base;
      lower--;
    }
  }

  int higher = lower + (2 * (num >= 0) - 1);

  int i = 0;
  long long j = 4;
  double initial = higher - lower;
  double estimate = initial / 2;
  int iterations = 120;
  int sig_figs = 0;

  while (i < iterations) {
    sig_figs = i/2;
    struct ImproperFraction f = double_to_fraction(estimate + lower);
    printf("%0.20f %lli %li\n", estimate + lower, f.numerator, f.denominator);
    double product = pow_frac(base, f);
    
    double delta = initial / j;
    if (product < num) {
      estimate += delta;
    } else {
      estimate -= delta;
    }
    
    // if (i > 11) {
    //   return estimate + lower;
    // }
    
    estimate = dround(estimate * pow_ll(10, sig_figs)) / pow_ll(10, sig_figs);

    j *= 2;
    i++;
  }

  return estimate + lower;
}
