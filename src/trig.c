#include "../include/trig.h"
#include "../include/enums.h"
#include "../include/utils.h"
#include "../include/futils.h"
#include "../auto-generated/cordic_constants.h"

// LUT-based
// float sine(float num) {
//   float x = fmodulo(num, PI * 2.0);

//   int idx = LENGTH / 2;
//   int iter = 4;
//   enum Boolean found = FALSE;

//   while (found == FALSE) {
//     float cur = SIN_LUT_A[idx];
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

//   float weight = (x - SIN_LUT_A[idx]) / (SIN_LUT_A[idx + 1] -
//   SIN_LUT_A[idx]);

//   return SIN_LUT_B[idx] + (SIN_LUT_B[idx + 1] - SIN_LUT_B[idx]) * weight;
// }

// CORDIC-based
struct SinCos angle_pair(float num) {
  float angle_sum = 0.0;
  float x = 1.0;
  float y = 0.0;

  struct SinCos out;

  unsigned int i = 0;
  while (i < CORDIC_COUNT) {
    float last_x = x;
    float last_y = y;

    float angle = CORDIC_CONSTANTS[i];
    if (angle == num) {
      // special logic
    }

    float halved_x = fhalve(x, i);
    float halved_y = fhalve(y, i);

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
