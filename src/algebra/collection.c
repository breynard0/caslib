#include "debug.h"
#include "enums.h"
#include "equation_objects.h"
#include "utils.h"
#include <stdio.h>

static int simplify_polyterm(struct EquationObject *buffer, int length);

struct MulVar {
  struct Letter letter;
  double degree;
};

Boolean eq_letter_equal(struct EquationObject self, struct EquationObject other,
                        double self_degree, double other_degree) {
  if (self.type != LETTER || other.type != LETTER) {
    return TRUE;
  }

  if (self.value.letter.letter != other.value.letter.letter ||
      self.value.letter.subscript != other.value.letter.subscript) {
    return FALSE;
  }

  if (self_degree != other_degree) {
    return FALSE;
  }

  return TRUE;
}

int collect_reorder_polynomial(struct EquationObject *buffer, int length) {
  int new_len = length;

  struct EquationObject mid_buf[new_len] = {};
  int mid_len = 0;

  // Collect factors within terms
  int i = 0;
  int count = 0;
  while (i < new_len) {
    if ((i == new_len - 1) || (buffer[i].type == END_LEX) ||
        (buffer[i].type == ADD) || (buffer[i].type == SUB)) {
      struct EquationObject tmp_buf[count];
      for (int j = 0; j < count; j++) {
        tmp_buf[j] = buffer[(i - count) + j];
      }

      int l = simplify_polyterm(tmp_buf, count);
      new_len -= count - l;

      // Insert into buffer
      for (int j = 0; j < l; j++) {
        mid_buf[mid_len] = tmp_buf[j];
        mid_len++;
      }
      if ((buffer[i].type == ADD) || (buffer[i].type == SUB)) {
        mid_buf[mid_len].type = buffer[i].type;
        mid_len++;
      }

      count = 0;
    } else {
      count++;
    }
    if (buffer[i].type == END_LEX) {
      break;
    }
    i++;
  }

  // Collect like terms
  // Yes, I know this method has awful time complexity. However, it has great
  // space complexity, and I can change it if it ends up being a bottleneck
  i = 0;
  count = 0;
  while (i < mid_len) {
    if ((mid_buf[i].type == ADD) || (mid_buf[i].type == SUB)) {
      int start = i - count;
      Boolean found = FALSE;
      Boolean correct = TRUE;

      int idx = i + 1;
      int idx_count = 0;
      int letters_found = 0;
      while (found == FALSE && idx < mid_len) {
        double self_degree = 1.0;
        if (mid_buf[start + idx_count + 1].type == EXP) {
          self_degree = mid_buf[start + idx_count + 2].value.number;
        }
        double other_degree = 1.0;
        if (mid_buf[idx + 1].type == EXP) {
          self_degree = mid_buf[idx + 2].value.number;
        }

        if (!eq_letter_equal(mid_buf[start + idx_count], mid_buf[idx],
                             self_degree, other_degree)) {
          correct = FALSE;
        }

        if (mid_buf[idx].type == LETTER) {
          letters_found++;
        }

        if (mid_buf[idx].type == ADD || mid_buf[idx].type == SUB ||
            idx >= mid_len - 1 || mid_buf[idx].type == END_LEX) {
          if (correct && letters_found != 0) {
            found = TRUE;
            for (int j = 0; j < count; j++) {
              remove_eo_idx(mid_buf, mid_len, start);
              mid_len--;
            }

            // Fold in negative sign if necessary
            if (mid_buf[start].type == SUB &&
                mid_buf[start + 1].type == NUMBER) {
              mid_buf[start + 1].value.number *= -1;
            }

            // Remove trailing addition or subtraction sign
            if (mid_len != 0) {
              remove_eo_idx(mid_buf, mid_len, start);
              mid_len--;
            }
            idx -= idx_count;
          }
          correct = TRUE;
          letters_found = 0;
          idx_count = -1;
        }

        idx_count++;
        idx++;
      }

      if (found) {
        i = 0;
      }
      count = 0;
    } else {
      count++;
    }

    if (i >= mid_len || buffer[i].type == END_LEX) {
      break;
    }

    i++;
  }

  mid_buf[mid_len].type = END_LEX;

  i = 0;
  while (i < mid_len) {
    buffer[i] = mid_buf[i];
    i++;
  }

  return mid_len;
}

int simplify_polyterm(struct EquationObject *buffer, int length) {
  struct EquationObject term[length + 1] = {};
  term[0].type = MULT;
  int iterator = 0;
  while (iterator < length) {
    term[iterator + 1] = buffer[iterator];
    iterator++;
  }

  length += 1;

  double consts = 1.0;
  struct MulVar vars[length] = {};
  int vars_len = 0;

  for (int i = 1; i < length + 1; i++) {
    if (term[i].type == NUMBER) {
      if (term[i - 1].type == MULT) {
        consts *= term[i].value.number;
      } else if (term[i - 1].type == DIV) {
        consts /= term[i].value.number;
      }
    } else if (term[i].type == LETTER) {
      struct Letter l = term[i].value.letter;
      int j = 0;
      Boolean found = FALSE;
      while (j < vars_len) {
        if (vars[j].letter.letter == l.letter &&
            vars[j].letter.subscript == l.subscript) {
          double my_degree = 1;
          if (i + 2 < length) {
            if (term[i + 1].type == EXP) {
              my_degree = term[i + 2].value.number;
            }
          }

          if (term[i - 1].type == MULT) {
            vars[j].degree += my_degree;
          }
          if (term[i - 1].type == DIV) {
            vars[j].degree -= my_degree;
          }

          found = TRUE;
          break;
        }
      }
      if (!found) {
        double my_degree = 1;
        if (i + 2 < length) {
          if (term[i + 1].type == EXP) {
            my_degree = term[i + 2].value.number;
          }
        }

        vars[vars_len].letter = l;
        if (term[i - 1].type == MULT) {
          vars[vars_len].degree += my_degree;
        }
        if (term[i - 1].type == DIV) {
          vars[vars_len].degree -= my_degree;
        }
        vars_len++;
      }
    }
  }

  // Estimate for longest possible length, refined later
  int out_buf_len = 2 * ((consts != 1.0) + (3 * vars_len)) + 1;
  struct EquationObject out_buf[out_buf_len];
  int out_buf_idx = 0;
  if (consts != 1.0) {
    out_buf[out_buf_idx].type = NUMBER;
    out_buf[out_buf_idx].value.number = consts;
    out_buf_idx++;
    if (vars_len != 0) {
      out_buf[out_buf_idx].type = MULT;
      out_buf_idx++;
    }
  }

  for (int j = 0; j < vars_len; j++) {
    // I think shouldn't crash because short circuiting?
    if ((out_buf_idx > 0) && out_buf[out_buf_idx - 1].type != MULT) {
      out_buf[out_buf_idx].type = MULT;
      out_buf_idx++;
    }

    out_buf[out_buf_idx].type = LETTER;
    out_buf[out_buf_idx].value.letter = vars[j].letter;
    out_buf_idx++;
    if (vars[j].degree != 1) {
      out_buf[out_buf_idx].type = EXP;
      out_buf_idx++;
      out_buf[out_buf_idx].type = NUMBER;
      out_buf[out_buf_idx].value.number = vars[j].degree;
      out_buf_idx++;
    }
  }

  // Refine length
  out_buf_len = out_buf_idx;

  for (int i = 0; i < out_buf_len; i++) {
    buffer[i] = out_buf[i];
  }

  return out_buf_len;
}
