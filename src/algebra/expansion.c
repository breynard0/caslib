#include "atrig.h"
#include "collection.h"
#include "debug.h"
#include "equation_objects.h"
#include "log.h"
#include "parse.h"
#include "pow.h"
#include "root.h"
#include "solve_consts.h"
#include "trig.h"
#include "utils.h"
#include <stdio.h>

// Make sure buffer is big enough! This function also assumes no block
// denominators.
int expand_polynomial(struct EquationObject *buffer, int length) {
  // Remove empty parentheses
  int par_idx = 1;
  while (par_idx < length) {
    if (buffer[par_idx].type == BLOCK_END &&
        buffer[par_idx - 1].type == BLOCK_START) {
      remove_eo_idx(buffer, length, par_idx);
      length--;
      remove_eo_idx(buffer, length, par_idx - 1);
      length--;
      par_idx -= 2;
    }
    par_idx++;
  }

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
    if ((buffer[i].type == NUMBER || buffer[i].type == LETTER ||
         buffer[i].type == BLOCK_START) &&
        buffer[i - 1].type == BLOCK_END) {
      int block_count = 1;
      int j = i - 2;
      int extra = 0;
      while (block_count != 0) {
        if (buffer[j].type == BLOCK_END) {
          block_count++;
        }
        if (buffer[j].type == BLOCK_START) {
          block_count--;
        }
        j--;
        extra++;
      }
      extra++;
      extra_count += extra * (length / extra);
    }
  }

  struct EquationObject expression[2 * (length + 2 * extra_count) + 1] = {};
  int new_len = expand_juxtopposed(buffer, length, expression,
                                   length + 2 * extra_count + 1, 0, 0);

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

  // Rearrange to make solvable
  i = 1;
  while (i < new_len) {
    struct EquationObject self = expression[i];
    struct EquationObject last = expression[i - 1];
    if (last.type == BLOCK_END && self.type == MULT) {
      int start = i - 2;
      int bracket_count = 1;
      while (bracket_count != 0) {
        if (expression[start].type == BLOCK_END) {
          bracket_count++;
        }
        if (expression[start].type == BLOCK_START) {
          bracket_count--;
        }
        start--;
      }
      start++;
      int factor_count = i - start;
      struct EquationObject factor[factor_count] = {};
      for (int j = 0; j < factor_count; j++) {
        factor[j] = expression[start];
        remove_eo_idx(expression, new_len, start);
        new_len--;
        i--;
      }

      int dest_start = i + 1;
      struct EquationObject mult_obj;
      mult_obj.type = MULT;

      if (expression[dest_start].type == LETTER ||
          expression[dest_start].type == NUMBER) {
        for (int j = 0; j < factor_count; j++) {
          new_len++;
          insert_eo_idx(expression, new_len, dest_start + j + 1, factor[j]);
        }
        new_len++;
        insert_eo_idx(expression, new_len, dest_start + 1, mult_obj);
      }

      // Remove leading multiplication sign
      remove_eo_idx(expression, new_len, i);
      new_len--;
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
          while (expression[prestart].type != ADD &&
                 expression[prestart].type != SUB) {
            if (prestart > 0) {
              prestart--;
            } else {
              break;
            }
          }
          if (expression[prestart].type == ADD ||
              expression[prestart].type == SUB) {
            prestart++;
          }
          int factor_len = start - prestart;
          struct EquationObject factor[factor_len];
          for (int n = 0; n < factor_len; n++) {
            factor[n] = expression[n + prestart];
          }

          // Removes starting block
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
