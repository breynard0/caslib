#include "equation_objects.h"
#include "parse.h"
#include "solve_consts.h"

// Make sure buffer is big enough! This function also assumes no block
// denominators.
int expand_polynomial(struct EquationObject *buffer, int length) {
  // Get number of juxtaposed elements to know how long to make buffer
  int extra_count = 0;
  for (int i = 1; i < length; i++) {
    if (is_juxtaposed(buffer[i], buffer[i - 1])) {
      extra_count++;
    }
    if (is_negative(buffer[i], buffer[i - 1])) {
      extra_count++;
    }
  }

  struct EquationObject expression[length + 2 * extra_count] = {};
  int new_len = expand_juxtopposed(buffer, length, expression,
                                   length + 2 * extra_count, 0, 0);

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
        expression[start].value.number = solve_const_expr(tmp_buf, i - start - 1, 0, 0);
        for (int j = 0; j < i - start; j++) {
          remove_eo_idx(expression, new_len, start + 1);
          new_len--;
        }
      }
    }

    i++;
  }
}
