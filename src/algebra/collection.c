#include "enums.h"
#include "equation_objects.h"
#include "utils.h"

static int simplify_polyterm(struct EquationObject *buffer, int length);

int collect_reorder_polynomial(struct EquationObject *buffer, int length) {
  int new_len = length;

  // Collect factors within terms
  int i = 0;
  int count = 0;
  while (i < new_len) {
    if (buffer[i].type == ADD || buffer[i].type == SUB) {
      struct EquationObject tmp_buf[count];
      for (int j = 0; j < count; j++) {
        tmp_buf[j] = buffer[(i - count) + j];
      }

      int l = simplify_polyterm(tmp_buf, count);
      new_len -= count - l;

      count = 0;
    } else {
      count++;
    }
    i++;
  }

  return 0;
}

struct MulVar {
  struct Letter letter;
  double degree;
};

static int simplify_polyterm(struct EquationObject *buffer, int length) {
  struct EquationObject term[length + 1] = {};
  term[0].type = MULT;
  for (int i = 0; i < length - 1; i++) {
    term[i + 1] = buffer[i];
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

          if (term[i - 1].type == ADD) {
            vars[j].degree += my_degree;
          }
          if (term[i - 1].type == SUB) {
            vars[j].degree -= my_degree;
          }

          found = TRUE;
          break;
        }
      }
    }
  }

  int out_buf_len = 2 * ((consts != 1.0) + (3 * vars_len));
  struct EquationObject out_buf[out_buf_len];
  int i = 0;
  if (consts != 1.0) {
    out_buf[i].type = NUMBER;
    out_buf[i].value.number = consts;
    i++;
    out_buf[i].type = MULT;
    i++;
  }

  for (int j = 0; j < vars_len; j++) {
    if (i != 0) {
      out_buf[i].type = MULT;
      i++;
    }
    out_buf[i].type = LETTER;
    out_buf[i].value.letter = vars[j].letter;
    i++;
    out_buf[i].type = EXP;
    i++;
    out_buf[i].type = NUMBER;
    out_buf[i].value.number = vars[j].degree;
    i++;
  }

  return out_buf_len;
}
