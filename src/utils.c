#include "../include/utils.h"

int int_abs(int num) {
  if (num < 0) {
    return -num;
  } else {
    return num;
  }
}

double double_abs(double num) {
  if (num < 0.0) {
    return -num;
  } else {
    return num;
  }
}

double pow_di(double num, int exponent) {
  if (exponent == 0) {
    return 1.0;
  }

  int abs_exp = int_abs(exponent);
  double val = 1.0;

  for (int i = 0; i < abs_exp; i++) {
    val *= num;
  }

  if (exponent > 0) {
    return val;
  } else {
    return 1.0/val;
  }
}

double dfloor(double num) {
    int round = (int) num;
    if (num < 0.0) {
        round -= 1;
    }
    return (double) round;
}

double dmodulo(double num, double mod) {
    double div = dfloor(double_abs(num / mod));
    double out = (num - (mod * div));
    if (num < 0) {
        return -out;
    } else {
        return out;
    }
}

double deg_to_rad(double degrees) {
    return PI * degrees / 180;
}
