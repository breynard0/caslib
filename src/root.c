#include "../include/dutils.h"
#include "../include/utils.h"
#include "../include/pow.h"

double nth_root(unsigned int n, double d) {
  // Generalization of Heron/Newton's method
  // Invalid check
  if ((n % 2) == 0 && d < 0) {
    return 0.0 / 0.0;
  }

  // Use double exponent to use square root of close-ish power of 2 as first
  // estimate
  double_cast cast = (double_cast)d;
  double estimate = pow_ll(2, ((cast.parts.exponent - 1023) / 2));

  double fn = (double)n;

  double small_mult = 1.0;
  if (d < 0.1 || d > 10000) {
    small_mult = 100.0;
  }

  // I've arbitrarily picked the number of iterations until it seems precise
  // enough
  const int ITERATIONS = 4;
  for (int i = 0; i < ITERATIONS * n * small_mult; i++) {
    estimate =
        (1.0 / fn) * ((fn - 1.0) * estimate + (d / pow_di(estimate, n - 1)));
  }

  if (n % 2 == 0) {
    return double_abs(estimate);
  } else {
    return estimate;
  }
}

double square_root(double d) { return nth_root(2, d); }
double cube_root(double d) { return nth_root(3, d); }
