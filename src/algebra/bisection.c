#include "equation_objects.h"
#include "isolation.h"
#include "solve_consts.h"
#include "utils.h"
#include "dutils.h"

double get_root_bisection(struct RootRange range,
                          struct EquationObject *expression, int length) {
  struct InputVar target;
  for (int i = 0; i < length; i++) {
    if (expression[i].type == LETTER) {
      target.letter = expression[i].value.letter;
      break;
    }
  }

  // Check for no roots in interval
  target.value = range.min;
  // double e = solve_const_expr(expression, length, &target, 1);
  short sign_min = solve_const_expr(expression, length, &target, 1) > 0;
  target.value = range.max;
  // double f = solve_const_expr(expression, length, &target, 1);
  short sign_max = solve_const_expr(expression, length, &target, 1) > 0;
  if (sign_min == sign_max) {
    return NAN;
  }

  if (range.min == range.max) {
    if (solve_const_expr(expression, length, &target, 1) == 0) {
      return range.max;
    } else {
      return NAN;
    }
  }

  double a = range.min;
  double b = range.max;
  double c = (range.min + range.max) / 2;

  double y = 1.0;

  while (double_abs(y) >= 0.000000001) {
    c = (a + b) / 2.0;

    target.value = a;
    short a_pos = solve_const_expr(expression, length, &target, 1) > 0;

    target.value = c;
    y = solve_const_expr(expression, length, &target, 1);
    if (y < 0.0) {
      if (a_pos) {
        b = c;
      } else {
        a = c;
      }
    } else if (y > 0.0) {
      if (a_pos) {
        a = c;
      } else {
        b = c;
      }
    }
    // printf("%f\n", c);
  }

  return c;
}
