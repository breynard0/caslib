#include <stdio.h>
double log_n(double num, double base) {
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
  
  int higher = lower + (2*(num >= 0)-1);
  
  printf("%i %i\n", lower, higher);

  return 0.0;
}
