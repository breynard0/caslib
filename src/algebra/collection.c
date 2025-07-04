#include "enums.h"
#include "equation_objects.h"
#include "utils.h"
#include "debug.h"
#include <stdio.h>

static int simplify_polyterm(struct EquationObject *buffer, int length);

int collect_reorder_polynomial(struct EquationObject *buffer, int length) {
  int new_len = length;
  
  struct EquationObject out_buf[new_len] = {};
  int out_len = 0;

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
        out_buf[out_len] = tmp_buf[j];
        out_len++;
      }
      if ((buffer[i].type == ADD) || (buffer[i].type == SUB)) {
        out_buf[out_len].type = buffer[i].type;
        out_len++;
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

  out_buf[out_len].type = END_LEX;
  
  return out_len;
}

struct MulVar {
  struct Letter letter;
  double degree;
};

int simplify_polyterm(struct EquationObject *buffer, int length) {
  struct EquationObject term[length + 1] = {};
  term[0].type = MULT;
  int iterator = 0;
  while (iterator < length) {
    term[iterator + 1] = buffer[iterator];
    iterator++;
  }

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
    out_buf[out_buf_idx].type = MULT;
    out_buf_idx++;
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
