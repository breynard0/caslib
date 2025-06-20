#include "../include/dutils.h"
#include "../include/enums.h"
#include "../include/root.h"
#include "../include/trig.h"
#include "../include/utils.h"

#include "../auto-generated/cordic_constants.h"
#include <stdio.h>

enum FuncType { SINE, COSINE, TANGENT };

static double arc_compute(double num, enum FuncType type) {
  short is_negative = 1;

  if (num < 0) {
    is_negative = -1;
  }

  num = double_abs(num);
  if (num > 1) {
    return 0.0 / 0.0;
  }

  // This method misbehaves for values very close to 1
  // Using rational equations from Desmos, though it does need to be solved
  // Thankfully, this happens to be a biquadratic equation, not a full quartic
  // Sin:
  // Cosine: y=0.0416603x^4+0.5x^2+1
  // Cosine: x=sqrt(abs((-0.5 + sqrt(0.1666412y+0.0833588))/0.0833206))
  const float threshold = 0.998;
  if (num > threshold) {
    if (type == COSINE) {
      printf("%0.150f\n", double_abs(-0.5 + nth_root(2, 0.1666412 * num + 0.0833588)) /
             0.0833206);
      return nth_root(
          2, double_abs(-0.5 + nth_root(2, 0.1666412 * num + 0.0833588)) /
                 0.0833206);
    }
  }

  double angle_sum = 0.0;
  double x = 1.0;
  double y = 0.0;

  int i = 0;
  while (i < CORDIC_COUNT) {
    double val;
    switch (type) {
    case SINE:
      val = y;
      break;
    case COSINE:
      val = x;
      break;
    case TANGENT:
      val = y / x;
      break;
    }

    double x_last = x;
    double y_last = y;
    double angle = CORDIC_CONSTANTS[i];

    double halved_x = dhalve(x, i);
    double halved_y = dhalve(y, i);

    printf("%i %f %f ", val < num, x, num);

    // Not using pre-computed values of cosine since Y is being directly
    // compared
    if (val < num) {
      x = (x_last - halved_y) * cosine(angle);
      y = (y_last + halved_x) * cosine(angle);
      angle_sum += angle;
    } else {
      x = (x_last + halved_y) * cosine(angle);
      y = (y_last - halved_x) * cosine(angle);
      angle_sum -= angle;
    }
    printf("%f\n", angle_sum);

    i++;
  }

  if (type == COSINE) {
    angle_sum = -angle_sum;
    if (is_negative == -1) {
      return PI - angle_sum;
    } else {
      return angle_sum;
    }
  }

  return angle_sum * is_negative;
}

double arc_sine(double num) { return arc_compute(num, SINE); }

double arc_cosine(double num) { return arc_compute(num, COSINE); }

double arc_tangent(double num) { return arc_compute(num, TANGENT); }
