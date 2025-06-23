#include "../include/utils.h"
#include "../include/gcf.h"

// Euclidean algorithm
long gcf(long x, long y) {
  x = long_abs(x);
  y = long_abs(y);
  
  while (x != 0 && y != 0) {
    if (x > y) {
      x = x % y;
    } else {
      y = y % x;
    }
  }
  return lmaximum(x, y);
}

void reduce_fraction(struct ImproperFraction* fraction) {
  long factor = gcf(fraction->numerator, fraction->denominator);
  fraction->numerator = fraction->numerator / factor;
  fraction->denominator = fraction->denominator / factor;
}
