#include "trig.h"
#include "dutils.h"
#include "enums.h"
#include "utils.h"

#include "../../auto-generated/cordic_constants.h"

// CORDIC implementation
struct SinCos angle_pair(double num) {
  Boolean negative = FALSE;
  if (num < 0) {
    negative = TRUE;
  }

  num = dmodulo(num, 2 * PI);

  if (negative == TRUE) {
    num = 2.0 * PI - num;
  }

  constexpr double halfpi = PI / 2.0;

  double x_mul = 1.0;
  double y_mul = 1.0;

  // Quadrant I
  if (num <= halfpi) {
    x_mul = 1.0;
    y_mul = 1.0;
  }
  // Quadrant II
  if (num > halfpi && num <= 2.0 * halfpi) {
    x_mul = -1.0;
    y_mul = 1.0;
    num = PI - num;
  }

  // Quadrant III
  if (num > PI && num <= 3.0 * halfpi) {
    x_mul = -1.0;
    y_mul = -1.0;
    num -= PI;
  }

  // Quadrant IV
  if (num > 3.0 * halfpi) {
    x_mul = 1.0;
    y_mul = -1.0;
    num = (2.0 * PI) - num;
  }

  double angle_sum = 0.0;
  double x = 1.0;
  double y = 0.0;

  struct SinCos out;

  unsigned int i = 0;
  while (i < CORDIC_COUNT) {
    const double last_x = x;
    const double last_y = y;

    const double angle = CORDIC_CONSTANTS[i];

    const double halved_x = dhalve(x, i);
    const double halved_y = dhalve(y, i);

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

  out.cos = x * COS_MULTIPLIER * x_mul;
  out.sin = y * COS_MULTIPLIER * y_mul;

  return out;
}

double sine(const double angle)
{
  const struct SinCos pair = angle_pair(angle);
  return pair.sin;
}

double cosine(const double angle)
{
  const struct SinCos pair = angle_pair(angle);
  return pair.cos;
}
double tangent(const double angle)
{
  const struct SinCos pair = angle_pair(angle);
  return pair.sin / pair.cos;
}
