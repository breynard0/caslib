#include "atrig.h"
#include "collection.h"
#include "cull.h"
#include "debug.h"
#include "equation_objects.h"
#include "log.h"
#include "parse.h"
#include "pow.h"
#include "root.h"
#include "solve_consts.h"
#include "trig.h"
#include "utils.h"

// Make sure buffer is big enough! This function also assumes no block
// denominators.
// Length should also be one less than the length of the buffer
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

  int new_len = 0;
  while (buffer[new_len].type != END_LEX) {
    new_len++;
  }
  new_len++;
  
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
      extra_count += extra * (new_len / extra);
    }
    if (buffer[i].type == EXP && buffer[i - 1].type == BLOCK_END) {
      extra_count += i * buffer[i + 1].value.number;
    }
  }

  int orig_expr_len = 2 * (new_len + 2 * extra_count) + 1;
  
  struct EquationObject expression[orig_expr_len] = {};
  
  new_len = expand_juxtopposed(buffer, new_len, expression,
                                   orig_expr_len, 0, 0);

  new_len = 0;
  while (expression[new_len].type != END_LEX) {
    new_len++;
  }
  new_len++;
  
  new_len = cull_the_useless(expression, new_len);

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

  new_len = cull_the_useless(expression, new_len);

  struct EquationObject mult_obj;
  mult_obj.type = MULT;

  // Expand term powers
  i = 1;
  while (i < new_len) {
    if (expression[i].type == EXP && expression[i - 1].type == BLOCK_END) {
      double exp = expression[i + 1].value.number;

      // Grab term
      int start = i - 2;
      while (expression[start].type != BLOCK_START) {
        start--;
      }

      int term_len = i - start;
      struct EquationObject term[term_len] = {};
      for (int j = 0; j < term_len; j++) {
        term[j] = expression[start + j];
      }

      // Remove exponent
      remove_eo_idx(expression, new_len, i);
      new_len--;
      remove_eo_idx(expression, new_len, i);
      new_len--;

      // Copy term
      for (int j = 0; j < exp - 1; j++) {
        new_len++;
        insert_eo_idx(expression, new_len, i, mult_obj);
        i++;
        for (int k = 0; k < term_len; k++) {
          new_len++;
          insert_eo_idx(expression, new_len, i, term[k]);
          i++;
        }
      }
      i = 0;
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

      // Replace subtraction with negative addition
      int k = 0;
      while (k < new_len) {
        if (expression[k].type == SUB) {
          expression[k].type = ADD;

          struct EquationObject obj;
          obj.type = NUMBER;
          obj.value.number = -1.0;

          new_len++;
          insert_eo_idx(expression, new_len, k + 1, mult_obj);
          new_len++;
          insert_eo_idx(expression, new_len, k + 1, obj);
        }
        k++;
      }

      int dest_start = i + 1;
      int end = -1;

      if (expression[dest_start].type == LETTER ||
          expression[dest_start].type == NUMBER) {
        for (int j = 0; j < factor_count; j++) {
          new_len++;
          insert_eo_idx(expression, new_len, dest_start + j + 1, factor[j]);
        }
        new_len++;
        insert_eo_idx(expression, new_len, dest_start + 1, mult_obj);
      }

      if (expression[dest_start].type == BLOCK_START) {
        int idx = dest_start + 1;
        Boolean running = TRUE;
        while (running) {
          if (expression[idx].type == BLOCK_END) {
            running = FALSE;
          }
          if (expression[idx].type == ADD || expression[idx].type == SUB ||
              expression[idx].type == BLOCK_END) {

            new_len++;
            insert_eo_idx(expression, new_len, idx, mult_obj);
            for (int j = 0; j < factor_count; j++) {
              idx++;
              new_len++;
              insert_eo_idx(expression, new_len, idx, factor[j]);
            }
            idx++;
          }
          idx++;
        }
        end = idx - 2;
      }

      // Remove leading multiplication sign
      remove_eo_idx(expression, new_len, i);
      new_len--;

      // Inline new thing, reusing buffer
      if (end > 0) {
        int idx = start;
        int j = 0;
        while (idx <= end) {
          buffer[j] = expression[idx];

          idx++;
          j++;
        }
        buffer[j].type = END_LEX;
        j++;
        int out_count = expand_polynomial(buffer, j) - 1;
        for (int k = 0; k < end - start; k++) {
          remove_eo_idx(expression, new_len, start);
          new_len--;
        }
        
        new_len++;
        struct EquationObject block_start;
        block_start.type = BLOCK_START;
        insert_eo_idx(expression, new_len, start, block_start);
        
        new_len += out_count;
        for (int k = 0; k < out_count; k++) {
          insert_eo_idx(expression, new_len, start + 1 + k, buffer[k]);
        }
      }

      i = -1;
    }
    i++;
  }

  new_len = cull_the_useless(expression, new_len);

  // Distributive property
  i = 0;
  while (i < new_len) {
    // while (0) {
    if (expression[i].type == BLOCK_END) {
      int start = i;
      while (expression[start].type != BLOCK_START) {
        start--;
      }

      if (start != 0) {
        if (expression[start - 1].type == MULT || expression[start - 1].type == DIV) {
          enum EOType op = expression[start - 1].type;
          int prestart = start - 2;
          while (expression[prestart].type != ADD &&
                 expression[prestart].type != SUB &&
                 expression[prestart].type != BLOCK_START) {
            if (prestart > 0) {
              prestart--;
            } else {
              break;
            }
          }
          if (expression[prestart].type == ADD ||
              expression[prestart].type == SUB ||
              expression[prestart].type == BLOCK_START) {
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
            
            if (expression[j].type == MULT && op == DIV) {
              expression[j].type = DIV;
            }

            j++;
          }
        }
      }
    }

    i++;
  }

  new_len = cull_the_useless(expression, new_len);

  // Add end_lex if it is missing
  if (expression[new_len - 1].type != END_LEX) {
    expression[new_len].type = END_LEX;
    new_len++;
  }

  new_len = 0;
  while (expression[new_len].type != END_LEX) {
    new_len++;
  }
  new_len++;
  
  // Collect terms
  new_len = collect_reorder_polynomial(expression, new_len);

  for (int m = 0; m < new_len; m++) {
    buffer[m] = expression[m];
  }
  return new_len;
}
