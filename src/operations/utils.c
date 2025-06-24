#include "../../include/utils.h"
#include "../../include/dutils.h"
#include "../../include/flags.h"
#include "../../include/gcf.h"
#include "../../include/root.h"
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

double deg_to_rad(double degrees) { return PI * degrees / 180; }

long lmaximum(long x, long y) {
  if (x > y) {
    return x;
  } else {
    return y;
  }
}
