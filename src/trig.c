#include "../include/trig.h"
#include "../include/enums.h"
#include "../include/utils.h"
#include "../include/dutils.h"
#include "../auto-generated/cordic_constants.h"

// LUT-based
// double sine(double num) {
//   double x = fmodulo(num, PI * 2.0);

//   int idx = LENGTH / 2;
//   int iter = 4;
//   enum Boolean found = FALSE;

//   while (found == FALSE) {
//     double cur = SIN_LUT_A[idx];
//     if ((cur <= x) && (SIN_LUT_A[idx + 1] >= x)) {
//       found = TRUE;
//     } else {
//       int correction = LENGTH / iter;
//       if (correction < 1) {
//         correction = 1;
//       }
//       if (cur < x) {
//         idx += correction;
//       } else {
//         idx -= correction;
//       }
//     }
//     iter *= 2;
//   }

//   double weight = (x - SIN_LUT_A[idx]) / (SIN_LUT_A[idx + 1] -
//   SIN_LUT_A[idx]);

//   return SIN_LUT_B[idx] + (SIN_LUT_B[idx + 1] - SIN_LUT_B[idx]) * weight;
// }

// CORDIC-based
struct SinCos angle_pair(double num) {
  double angle_sum = 0.0;
  double x = 1.0;
  double y = 0.0;

  struct SinCos out;

  unsigned int i = 0;
  while (i < CORDIC_COUNT) {
    double last_x = x;
    double last_y = y;

    double angle = CORDIC_CONSTANTS[i];
    if (angle == num) {
      // special logic
    }

    double halved_x = dhalve(x, i);
    double halved_y = dhalve(y, i);

    if (angle_sum > num) {
        angle_sum -= angle;
        x = last_x + halved_y;
        y = last_y - halved_x;
    } else {
        angle_sum += angle;
        x = last_x - halved_y;
        y = last_y + halved_x;
    }

    i++;
  }

  out.cos = x * COS_MULTIPLIER;
  out.sin = y * COS_MULTIPLIER;

  return out;
}
