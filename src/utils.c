#include "../include/utils.h"

int int_abs(int num) {
  if (num < 0) {
    return -num;
  } else {
    return num;
  }
}

float float_abs(float num) {
  if (num < 0.0) {
    return -num;
  } else {
    return num;
  }
}

float pow_fi(float num, int exponent) {
  if (exponent == 0) {
    return 1.0;
  }

  int abs_exp = int_abs(exponent);
  float val = 1.0;

  for (int i = 0; i < abs_exp; i++) {
    val *= num;
  }

  if (exponent > 0) {
    return val;
  } else {
    return 1.0/val;
  }
}

float ffloor(float num) {
    int round = (int) num;
    if (num < 0.0) {
        round -= 1;
    }
    return (float) round;
}

float fmodulo(float num, float mod) {
    float div = ffloor(float_abs(num / mod));
    float out = (num - (mod * div));
    if (num < 0) {
        return -out;
    } else {
        return out;
    }
}

float deg_to_rad(float degrees) {
    return PI * degrees / 180;
}
