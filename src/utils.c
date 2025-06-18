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