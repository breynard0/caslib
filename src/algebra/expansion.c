#include "atrig.h"
#include "equation_objects.h"
#include "log.h"
#include "parse.h"
#include "pow.h"
#include "root.h"
#include "solve_consts.h"
#include "trig.h"
#include "utils.h"
#include "collection.h"

// Make sure buffer is big enough! This function also assumes no block
// denominators.
int expand_polynomial(struct EquationObject *buffer, int length) {
  // Get number of juxtaposed elements to know how long to make buffer
  int extra_count = 0;
  for (int i = 1; i < length; i++) {
    if (buffer[i].type == END_LEX) {
      break;
    }
    if (is_juxtaposed(buffer[i], buffer[i - 1])) {
      extra_count++;
    }
    if (is_negative(buffer[i], buffer[i - 1])) {
      extra_count++;
    }
  }

  struct EquationObject expression[2 * (length + 2 * extra_count)] = {};
  int new_len = expand_juxtopposed(buffer, length, expression,
                                   length + 2 * extra_count, 0, 0);

  struct EquationObject test;
  // test.type = ADD;
  // insert_eo_idx(expression, new_len, 4, test);
  // buffer[0] = test;
  // return 1;

  // Evaluate constant blocks
  int i = 0;
  while (i < new_len) {
    struct EquationObject cur = expression[i];

    if (cur.type == BLOCK_END) {
      int start = i - 1;
      int count = 1;
      Boolean vars = FALSE;

      while (count != 0) {
        if (expression[start].type == BLOCK_START) {
          count--;
        } else if (expression[start].type == BLOCK_END) {
          count++;
        } else if (expression[start].type == LETTER) {
          vars = TRUE;
        }
        if (count != 0) {
          start--;
        }
      }

      if (!vars) {
        struct EquationObject tmp_buf[i - start - 1];
        for (int j = 1; j < i - start; j++) {
          tmp_buf[j - 1] = expression[start + j];
        }

        expression[start].type = NUMBER;
        expression[start].value.number =
            solve_const_expr(tmp_buf, i - start - 1, 0, 0);
        for (int j = 0; j < i - start; j++) {
          remove_eo_idx(expression, new_len, start + 1);
          new_len--;
        }
      }
    }

    i++;
  }

  // Evaluate functions
  i = 0;
  while (i < new_len) {
    struct EquationObject cur = expression[i];

    switch (cur.type) {
    case PI_VAL:
      expression[i].type = NUMBER;
      expression[i].value.number = PI;
      break;
    case ROOT:
      expression[i].type = NUMBER;
      expression[i].value.number = nth_root(expression[i - 1].value.number,
                                            expression[i + 1].value.number);
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      break;
    case EXP:
      if (expression[i - 1].type == NUMBER) {
        expression[i].type = NUMBER;
        expression[i].value.number = pow_dd(expression[i - 1].value.number,
                                            expression[i + 1].value.number);
        remove_eo_idx(expression, new_len, i + 1);
        new_len--;
        remove_eo_idx(expression, new_len, i - 1);
        new_len--;
      }
      break;
    case SINE:
      expression[i].type = NUMBER;
      expression[i].value.number = sine(expression[i + 1].value.number);
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      break;
    case COSINE:
      expression[i].type = NUMBER;
      expression[i].value.number = cosine(expression[i + 1].value.number);
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      break;
    case TANGENT:
      expression[i].type = NUMBER;
      expression[i].value.number = tangent(expression[i + 1].value.number);
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      break;
    case ARCSINE:
      expression[i].type = NUMBER;
      expression[i].value.number = arc_sine(expression[i + 1].value.number);
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      break;
    case ARCCOSINE:
      expression[i].type = NUMBER;
      expression[i].value.number = arc_cosine(expression[i + 1].value.number);
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      break;
    case ARCTANGENT:
      expression[i].type = NUMBER;
      expression[i].value.number = arc_tangent(expression[i + 1].value.number);
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      break;
    case LOG:
      expression[i].type = NUMBER;
      expression[i].value.number =
          log_n(expression[i - 1].value.number, expression[i + 1].value.number);
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      break;
    default:
      break;
    }

    i++;
  }

  // Distributive property
  i = 0;
  while (i < new_len) {
    if (expression[i].type == BLOCK_END) {
      int start = i;
      while (expression[start].type != BLOCK_START) {
        start--;
      }

      if (start != 0) {
        if (expression[start - 1].type == MULT) {
          int prestart = start - 2;
          while (expression[prestart].type != ADD ||
                 expression[prestart].type != SUB) {
            if (prestart > 0) {
              prestart--;
            } else {
              break;
            }
          }
          int factor_len = start - prestart;
          struct EquationObject factor[factor_len];
          for (int n = prestart; n < factor_len; n++) {
            factor[n] = expression[n + prestart];
          }

          remove_eo_idx(expression, new_len, start);
          new_len--;
          int j = start;
          while (j < new_len) {
            if (expression[j].type == BLOCK_END) {
              remove_eo_idx(expression, new_len, j);
              new_len--;
              break;
            }

            if (expression[j].type == ADD || expression[j].type == SUB) {
              j++;
              for (int k = 0; k < factor_len; k++) {
                new_len++;
                insert_eo_idx(expression, new_len, j + k, factor[k]);
              }
            }

            j++;
          }
        }
      }
    }

    i++;
  }

  // Collect terms
  new_len = collect_reorder_polynomial(expression, new_len);
  
  for (int m = 0; m < new_len; m++) {
    buffer[m] = expression[m];
  }
  return new_len;
}
