#include "utils.h"
#include "dutils.h"
#include "flags.h"
#include "gcf.h"
#include "log.h"
#include "pow.h"
#include "root.h"

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

long long long_long_abs(long long num) {
  if (num < 0) {
    return -num;
  } else {
    return num;
  }
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

short double_digits_whole(double num) { return ((short)log_n(num, 10)) + 1; }

static short check_places(double num, short places) {
  short i = 0;
  // struct ImproperFraction f = double_to_fraction(num - dfloor(num));
  // printf("%lli\n", f.numerator);
  // while (i < places) {
  //   double n = num * pow_di(10, i);
  //   // printf("%f, ", dmodulo(dround(n), 10));
  //   if (dmodulo(dround(n), 10) != 0) {
  //     // printf("\n");
  //     return 1;
  //   }

  //   i++;
  // }
  // // printf("\n");
  // return 0;
  return 1;
}

short double_digits_partial(double num) {
  short digits = 0;
  while (dmodulo(num * pow_di(10, digits), 1.0) != 0 &&
         check_places(num * pow_di(10, digits), 3)) {
    digits++;
  }

  return digits;
}

double deg_to_rad(double degrees) { return PI * degrees / 180; }

int imaximum(int x, int y) {
  if (x > y) {
    return x;
  } else {
    return y;
  }
}

long lmaximum(long x, long y) {
  if (x > y) {
    return x;
  } else {
    return y;
  }
}

double round_to_threshold(double num) {
  double rounded = dround(num);
  double checked = double_abs(num - rounded);
  if (double_abs(rounded - num) <= THRESHOLD) {
    return rounded;
  }
  return num;
}
