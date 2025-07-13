#include "get_terms.h"
#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"

int get_terms(struct EquationObject *input, int input_len,
              struct Term *term_buf, int term_max_len,
              struct EquationObject *data_buf, int data_max_len,
              Boolean simplify) {
  int i = 0;
  struct EquationObject cur_term_val[input_len * 2] = {};
  int cur_term_len = 0;
  int block_count = 0;
  int sign = 1;

  int out_len = 0;

  while (i < input_len) {
    struct EquationObject cur = input[i];
    if ((block_count == 0 && (cur.type == ADD || cur.type == SUB)) ||
        i >= input_len - 1) {
      cur_term_val[cur_term_len].type = END_LEX;
      cur_term_len++;

      int inner_block_count = 0;
      int denom_sep = 0;
      while (cur_term_val[denom_sep].type != DIV || inner_block_count != 0) {
        if (denom_sep >= cur_term_len) {
          denom_sep = -1;
          break;
        }
        if (cur_term_val[denom_sep].type == BLOCK_START) {
          inner_block_count++;
        }
        if (cur_term_val[denom_sep].type == BLOCK_END) {
          inner_block_count--;
        }
        denom_sep++;
      }

      term_buf[out_len].numerator_length = denom_sep;
      if (denom_sep == -1) {
        term_buf[out_len].numerator_length = cur_term_len;
        term_buf[out_len].denominator_length = 0;
      } else {
        term_buf[out_len].denominator_length = cur_term_len - denom_sep - 2;
      }

      struct EquationObject data[term_buf[out_len].numerator_length +
                                 term_buf[out_len].denominator_length] = {};
      for (int j = 0; j < term_buf[out_len].numerator_length; j++) {
        data[j] = cur_term_val[j];
      }

      if (denom_sep != -1) {
        for (int j = 0; j < term_buf[out_len].denominator_length; j++) {
          data[j + denom_sep] = cur_term_val[j + 1 + denom_sep];
        }
      }
      term_buf[out_len].data = &data;

      out_len++;

      if (cur.type == SUB) {
        sign = -1;
      } else {
        sign = 1;
      }

      cur_term_len = 0;
      i++;
      continue;
    }

    if (cur.type == BLOCK_START) {
      block_count++;
    }
    if (cur.type == BLOCK_END) {
      block_count--;
    }

    cur_term_val[cur_term_len] = cur;
    cur_term_len++;
    i++;
  }

  return out_len;
}
