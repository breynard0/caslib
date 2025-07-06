#include "solve_consts.h"
#include "atrig.h"
#include "dutils.h"
#include "enums.h"
#include "equation_objects.h"
#include "flags.h"
#include "log.h"
#include "parse.h"
#include "pow.h"
#include "root.h"
#include "trig.h"
#include "utils.h"

double solve_const_expr(struct EquationObject *input, int length,
                        struct InputVar *buffer, int num_args) {
  // Get number of juxtaposed elements to know how long to make buffer
  int extra_count = 0;
  for (int i = 1; i < length; i++) {
    if (is_juxtaposed(input[i], input[i - 1])) {
      extra_count++;
    }
    if (is_negative(input[i], input[i - 1])) {
      extra_count++;
    }
  }

  struct EquationObject expression[length + 2 * extra_count] = {};
  int new_len = expand_juxtopposed(input, length, expression, length + 2 * extra_count, buffer, num_args);

  // Use recursion to reduce blocks to doubles
  int i = 0;
  Boolean blocks_found = FALSE;
  while (!blocks_found) {
    if (i >= new_len) {
      blocks_found = TRUE;
      break;
    }

    if (expression[i].type == BLOCK_END) {
      int start = i;
      while (start >= 0) {
        if (expression[start].type == BLOCK_START) {
          break;
        } else {
          start--;
        }
      }
      short count = i - start;
      struct EquationObject new_buffer[count];
      short len = 0;

      // Remove start block
      remove_eo_idx(expression, new_len, start);
      new_len--;

      // New buffer and recursion
      for (int j = 0; j < count - 1; j++) {
        new_buffer[j] = expression[start];
        len++;
        remove_eo_idx(expression, new_len, start);
        new_len--;
      }
      expression[start].type = NUMBER;
      expression[start].value.number =
          solve_const_expr(new_buffer, len, buffer, num_args);
      i -= count;
    }
    i++;
  }

  // Solve the rest in PEDMAS order

  // Functions
  // Going backwards because nested functions
  for (int i = new_len - 2; i >= 0; i--) {
    Boolean exit = FALSE;
    double val = 0.0;
    if (i + 1 < new_len) {
      switch (expression[i].type) {
      case SINE:
        val = sine(expression[i + 1].value.number);
        break;
      case COSINE:
        val = cosine(expression[i + 1].value.number);
        break;
      case TANGENT:
        val = tangent(expression[i + 1].value.number);
        break;
      case ARCSINE:
        val = arc_sine(expression[i + 1].value.number);
        break;
      case ARCCOSINE:
        val = arc_cosine(expression[i + 1].value.number);
        break;
      case ARCTANGENT:
        val = arc_tangent(expression[i + 1].value.number);
        break;
      default:
        exit = TRUE;
        break;
      }
    }

    if (exit) {
      continue;
    }

    expression[i].type = NUMBER;
    expression[i].value.number = val;

    if (i + 1 >= new_len) {
      f_bad_equation = 1;
      return NAN;
    }
    remove_eo_idx(expression, new_len, i + 1);
    new_len--;
  }

  // Exponentiation and friends
  i = 1;
  Boolean exp_found = FALSE;
  while (!exp_found) {
    if (i >= new_len) {
      exp_found = TRUE;
      break;
    }

    double before = expression[i - 1].value.number;
    double after = expression[i + 1].value.number;
    double val = 0.0;

    Boolean found = FALSE;

    if (expression[i].type == EXP) {
      if (dfloor(expression[i + 1].value.number) ==
          expression[i + 1].value.number) {
        val = pow_di(expression[i - 1].value.number,
                     (int)expression[i + 1].value.number);
      } else {
        val = pow_dd(expression[i - 1].value.number,
                     expression[i + 1].value.number);
      }
      found = TRUE;
    }

    if (expression[i].type == ROOT) {
      val = nth_root(expression[i - 1].value.number,
                     expression[i + 1].value.number);

      found = TRUE;
    }

    if (expression[i].type == LOG) {
      val =
          log_n(expression[i - 1].value.number, expression[i + 1].value.number);

      found = TRUE;
    }

    if (found) {
      expression[i].type = NUMBER;
      expression[i].value.number = val;
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      i = 1;
      continue;
    }
    i++;
  }

  // Multiplications and divisions
  i = 1;
  Boolean mult_found = FALSE;
  while (!mult_found) {
    if (i >= new_len) {
      mult_found = TRUE;
      break;
    }
    double before = expression[i - 1].value.number;
    double after = expression[i + 1].value.number;
    double val = 0.0;

    Boolean found = FALSE;

    if (expression[i].type == MULT) {
      val = before * after;
      found = TRUE;
    }
    if (expression[i].type == DIV) {
      val = before / after;
      found = TRUE;
    }

    if (found) {
      expression[i].type = NUMBER;
      expression[i].value.number = val;
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      i = 1;
      continue;
    }
    i++;
  }

  // Addition and subtraction
  i = 1;
  Boolean add_found = FALSE;
  while (!add_found) {
    if (i >= new_len) {
      add_found = TRUE;
      break;
    }

    double before = expression[i - 1].value.number;
    double after = expression[i + 1].value.number;
    double val = 0.0;

    Boolean found = FALSE;

    if (expression[i].type == ADD) {
      val = before + after;
      found = TRUE;
    }
    if (expression[i].type == SUB) {
      val = before - after;
      found = TRUE;
    }

    if (found) {
      expression[i].type = NUMBER;
      expression[i].value.number = val;
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      i = 1;
      continue;
    }
    i++;
  }

  return expression[0].value.number;
}
