#include "../include/utils.h"
#include "../include/flags.h"
#include "../include/pow.h"

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
  
  int higher = lower + (2*(num >= 0)-1);
  
  int i = 0;
  int j = 2;
  double initial = (pow_di(num, higher) - pow_di(num, lower)) / 2.0;
  double estimate = initial;
  int iterations = 20 * lower;
  
  while (i < iterations) {
    
    j *= 2;
    i++;
  }

  return 0.0;
}
