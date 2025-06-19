#include "../auto-generated/tan_lut.h"
#include "../include/utils.h"
#include "../include/enums.h"

#define FLT_MAX 3.402823466e+38F

float tangent(float num) {
  // modulo in future
  float x = num;

  int idx = LENGTH / 2;
  int iter = 4;
  enum Boolean found = FALSE;

  while (found == FALSE) {
    float cur = TAN_LUT_A[idx];
    if ((cur <= x) && (TAN_LUT_A[idx + 1] > x)) {
      found = TRUE;
    } else {
      int correction = LENGTH / iter;
      if (correction < 1) {
        correction = 1;
      }
      if (cur < x) {
        if (idx >= LENGTH) {
          return FLT_MAX;
        }
        idx += correction;
      } else {
        if (idx <= 0) {
          return 0.0;
        }
        idx -= correction;
      }
    }
    iter *= 2;
  }

  float weight = (x - TAN_LUT_A[idx]) / (TAN_LUT_A[idx + 1] - TAN_LUT_A[idx]);

  return TAN_LUT_B[idx] + (TAN_LUT_B[idx + 1] - TAN_LUT_B[idx]) * weight;
}