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

void remove_mv_idx(struct MulVar *list, int length, int idx) {
  for (int i = idx; i < length - 1; i++) {
    list[i] = list[i + 1];
  }
}
void remove_d_idx(double *list, int length, int idx) {
  for (int i = idx; i < length - 1; i++) {
    list[i] = list[i + 1];
  }
}

int collect_reorder_polynomial(struct EquationObject *buffer, int length) {
  int new_len = length;
  struct EquationObject start_buf[new_len] = {};
  for (int i = 0; i < new_len; i++) {
    start_buf[i] = buffer[i];
  }

  struct EquationObject mid_buf[new_len] = {};
  int mid_len = 0;

  // TODO: refactor this into its own file, this function is getting long
  // Remove redundant parentheses
  int i = 0;
  while (i < new_len) {
    if (start_buf[i].type == BLOCK_END) {
      int start = i - 1;
      int blocks = 1;
      while (blocks != 0) {
        if (start_buf[start].type == BLOCK_START) {
          blocks--;
        }
        if (start_buf[start].type == BLOCK_END) {
          blocks++;
        }
        start--;
      }
      start++;

      Boolean is_before =
          start != 0 && (start_buf[start - 1].type == ADD ||
                         start_buf[start - 1].type == SUB ||
                         start_buf[start - 1].type == BLOCK_START);
      Boolean is_after =
          i < new_len - 1 &&
          (start_buf[i + 1].type == ADD || start_buf[i + 1].type == SUB ||
           start_buf[i + 1].type == BLOCK_END);

      if ((is_before && is_after) || (start == 0 && is_after) ||
          ((i >= new_len || start_buf[i + 1].type == END_LEX) && is_before) ||
          (start == 0 && (i >= new_len || start_buf[i + 1].type == END_LEX))) {
        remove_eo_idx(start_buf, new_len, i);
        new_len--;
        remove_eo_idx(start_buf, new_len, start);
        new_len--;
        i -= 2;
      }
    }

    i++;
  }

  // Collect factors within terms
  i = 0;
  int count = 0;
  while (i < new_len) {
    if ((i == new_len - 1) || (start_buf[i].type == END_LEX) ||
        (start_buf[i].type == ADD) || (start_buf[i].type == SUB)) {
      struct EquationObject tmp_buf[count];
      for (int j = 0; j < count; j++) {
        tmp_buf[j] = start_buf[(i - count) + j];
      }

      int l = simplify_polyterm(tmp_buf, count);
      new_len -= count - l;

      // Insert into buffer
      for (int j = 0; j < l; j++) {
        mid_buf[mid_len] = tmp_buf[j];
        mid_len++;
      }
      if ((start_buf[i].type == ADD) || (start_buf[i].type == SUB)) {
        mid_buf[mid_len].type = start_buf[i].type;
        mid_len++;
      }

      count = 0;
    } else {
      count++;
    }
    if (start_buf[i].type == END_LEX) {
      break;
    }
    i++;
  }

  // Collect like terms

  // NaN degree denotes separation
  struct MulVar vars[mid_len] = {};
  int vars_len = 0;

  double coeffs[mid_len] = {};
  int coeffs_len = 0;

  // Construct arrays
  i = 0;
  while (i < mid_len) {
    enum EOType t = mid_buf[i].type;
    if (t == NUMBER) {
      coeffs[coeffs_len] = mid_buf[i].value.number;
      coeffs_len++;
    }
    if (t == LETTER) {
      vars[vars_len].letter = mid_buf[i].value.letter;

      double degree = 1;
      if (i < mid_len - 1 && mid_buf[i + 1].type == EXP) {
        i += 2;
        degree = mid_buf[i].value.number;
      }
      vars[vars_len].degree = degree;
      vars_len++;

      // Add coefficient of 1 if does not exist
      if (i < 2 || (i >= 2 && ((mid_buf[i - 1].type == ADD ||
                                mid_buf[i - 1].type == SUB) ||
                               mid_buf[i - 2].type != NUMBER))) {
        coeffs[coeffs_len] = 1.0;
        coeffs_len++;
      }
    }
    if (t == ADD || t == SUB) {
      vars[vars_len].degree = NAN;
      vars_len++;
      if (t == SUB) {
        if (mid_buf[i + 1].type == NUMBER) {
          mid_buf[i + 1].value.number *= -1;
        } else if (mid_buf[i + 1].type == LETTER) {
          mid_len++;
          struct EquationObject obj;
          obj.type = NUMBER;
          obj.value.number = -1.0;
          insert_eo_idx(mid_buf, mid_len, i + 1, obj);
        }
      }
    }
    i++;
  }
  vars[vars_len].degree = NAN;
  vars_len++;

  struct MulVar out_vars[vars_len + coeffs_len] = {};
  int out_vars_len = 0;

  double coeffs_out[coeffs_len] = {};
  int out_coeffs_len = 0;

  i = 0;
  count = 0;
  int n = 0;
  double coeff = coeffs[n];
  while (i < vars_len) {
    // Check if NaN
    if (vars[i].degree != vars[i].degree) {
      int start = i - count;
      int j = i + 1;
      int j_count = 0;
      int local_n = n + 1;
      Boolean correct = TRUE;
      int vars_compared = 0;

      while (j < vars_len) {
        if (vars[j].degree != vars[j].degree) {
          // Check for mismatch
          if (vars[start + j_count].degree == vars[start + j_count].degree) {
            correct = FALSE;
          }
          if (correct && (vars_compared != 0 || count == 0)) {
            coeff += coeffs[local_n];
            // Remove term and one separator
            int term_start = j - j_count;
            for (int k = 0; k < j_count + 1; k++) {
              remove_mv_idx(vars, vars_len, term_start);
              vars_len--;
              j--;
            }
            remove_d_idx(coeffs, coeffs_len, local_n);
            coeffs_len--;
            local_n--;
          }

          j_count = 0;
          local_n++;
          j++;
          correct = TRUE;
          vars_compared = 0;
          continue;
        }

        if (correct) {
          struct MulVar self = vars[start + j_count];
          struct MulVar other = vars[j];

          if (self.letter.letter != other.letter.letter ||
              self.letter.subscript != other.letter.subscript ||
              self.degree != other.degree) {
            correct = FALSE;
          }

          vars_compared++;
        }
        j_count++;
        j++;
      }

      // Push here
      coeffs_out[out_coeffs_len] = coeff;
      out_coeffs_len++;
      if (i != 0 && vars[i - 1].degree != vars[i - 1].degree) {
        out_vars[out_vars_len].letter.letter = 'x';
        out_vars[out_vars_len].letter.subscript = ' ';
        out_vars[out_vars_len].degree = 0;
        out_vars_len++;
      }

      if (i != 0) {
        int h = i - 1;
        while (vars[h].degree == vars[h].degree && h != 0) {
          h--;
        }
        if (vars[h].degree != vars[h].degree) {
          h++;
        }
        while (vars[h].degree == vars[h].degree && h < vars_len) {
          out_vars[out_vars_len] = vars[h];
          out_vars_len++;

          h++;
        }
        out_vars[out_vars_len].degree = NAN;
        out_vars_len++;
      }

      n++;
      coeff = coeffs[n];
      count = -1;
    }

    count++;
    i++;
  }

  out_vars_len--;

  struct EquationObject out_buf[length] = {};
  int out_buf_len = 0;

  while (out_vars_len > 0) {
    i = 0;
    count = 0;
    n = 0;

    int max = 0;
    int max_n = 0;

    while (i < out_vars_len) {
      if (out_vars[i].degree > max) {
        max = out_vars[i].degree;
        max_n = n;
      }

      if (out_vars[i].degree != out_vars[i].degree) {
        n++;
      }

      i++;
    }

    if (coeffs_out[max_n] != 1) {
      out_buf[out_buf_len].type = NUMBER;
      out_buf[out_buf_len].value.number = coeffs_out[max_n];
      out_buf_len++;
      out_buf[out_buf_len].type = MULT;
      out_buf_len++;
    }
    remove_d_idx(coeffs_out, out_coeffs_len, max_n);
    out_coeffs_len--;

    i = 0;
    int local_n = 0;
    while (local_n < max_n) {
      if (out_vars[i].degree != out_vars[i].degree) {
        local_n++;
      }
      i++;
    }
    if (out_vars[i].degree != out_vars[i].degree) {
      i++;
    }
    while (out_vars[i].degree == out_vars[i].degree && i < out_vars_len) {
      out_buf[out_buf_len].type = LETTER;
      out_buf[out_buf_len].value.letter = out_vars[i].letter;
      out_buf_len++;
      if (out_vars[i].degree != 1) {
        out_buf[out_buf_len].type = EXP;
        out_buf_len++;
        out_buf[out_buf_len].type = NUMBER;
        out_buf[out_buf_len].value.number = out_vars[i].degree;
        out_buf_len++;
      }

      if (i != out_vars_len - 1 &&
          out_vars[i + 1].degree == out_vars[i + 1].degree) {
        out_buf[out_buf_len].type = MULT;
      } else {
        out_buf[out_buf_len].type = ADD;
      }
      out_buf_len++;

      // Remove self
      remove_mv_idx(out_vars, out_vars_len, i);
      out_vars_len--;
    }
    // Remove separator
    remove_mv_idx(out_vars, out_vars_len, i);
    out_vars_len--;
  }

  // Replace trailing add with endlex
  out_buf[out_buf_len - 1].type = END_LEX;

  // Fix some stuff, make negatives subtraction and remove x^0
  for (i = 1; i < out_buf_len; i++) {
    // Negative stuff
    if (out_buf[i].type == NUMBER && out_buf[i].value.number < 0) {
      out_buf[i].value.number *= -1;
      if (out_buf[i - 1].type == ADD) {
        out_buf[i - 1].type = SUB;
      } else if (out_buf[i - 1].type == SUB) {
        out_buf[i - 1].type = ADD;
      }
    }
    // x^0
    if (out_buf[i].type == NUMBER && out_buf[i].value.number == 0 &&
        out_buf[i - 1].type == EXP && out_buf[i - 2].type == LETTER) {
      for (int j = 0; j < 4; j++) {
        remove_eo_idx(out_buf, out_buf_len, i - 3);
        out_buf_len--;
      }
    }
  }

  for (int l = 0; l < out_buf_len; l++) {
    buffer[l] = out_buf[l];
  }

  return out_buf_len;
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

  for (int i = 1; i < length; i++) {
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
        j++;
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
