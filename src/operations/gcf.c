#include "gcf.h"
#include "equation_objects.h"
#include "utils.h"

// Euclidean algorithm
long gcf(long x, long y) {
  x = long_abs(x);
  y = long_abs(y);

  while (x != 0 && y != 0) {
    if (x > y) {
      x = x % y;
    } else {
      y = y % x;
    }
  }
  return lmaximum(x, y);
}

void reduce_fraction(struct ImproperFraction *fraction) {
  long factor = gcf(fraction->numerator, fraction->denominator);
  fraction->numerator = fraction->numerator / factor;
  fraction->denominator = fraction->denominator / factor;
}

int term_gcf(struct EquationObject *term0, int t0_len,
             struct EquationObject *term1, int t1_len,
             struct EquationObject *buffer, int max_len,
             struct Letter exclude) {
  // Push number
  int new_len = 0;
  double t0_coeff = 1.0;
  double t1_coeff = 1.0;
  if (term0[0].type == NUMBER) {
    t0_coeff = term0[0].value.number;
  }
  if (term1[0].type == NUMBER) {
    t1_coeff = term1[0].value.number;
  }
  if (new_len < max_len) {
    if (t0_coeff != (int)t0_coeff) {
      t0_coeff = 1.0;
    }
    if (t1_coeff != (int)t1_coeff) {
      t1_coeff = 1.0;
    }
    buffer[new_len].type = NUMBER;
    buffer[new_len].value.number = gcf(t0_coeff, t1_coeff);
    new_len++;
    buffer[new_len].type = MULT;
    new_len++;
  }

  // Push variables
  int i = 0;
  while (i < t0_len) {
    if (term0[i].type == LETTER &&
        !(term0[i].value.letter.letter == exclude.letter &&
          term0[i].value.letter.subscript == exclude.subscript)) {
      char ex = term0[i].value.letter.letter;
      double self_power = 1.0;
      if (i < t0_len && term0[i + 1].type == EXP) {
        self_power = term0[i + 2].value.number;
      }
      int j = 0;
      while (term1[j].type != LETTER ||
             term1[j].value.letter.letter != term0[i].value.letter.letter ||
             term1[j].value.letter.subscript !=
                 term0[i].value.letter.subscript) {
        if (j >= t1_len - 1) {
          j = -1;
          break;
        }
        j++;
      }
      if (j >= 0) {
        double other_power = 1.0;
        if (j < t1_len && term1[j + 1].type == EXP) {
          other_power = term1[j + 2].value.number;
        }
        double min = self_power;
        if (other_power < self_power) {
          min = other_power;
        }
        if (min != 0.0 && new_len < max_len - 1) {
          buffer[new_len].type = LETTER;
          buffer[new_len].value.letter = term0[i].value.letter;
          new_len++;

          if (min != 1.0 && new_len < max_len - 1) {
            buffer[new_len].type = EXP;
            new_len++;
            buffer[new_len].type = NUMBER;
            buffer[new_len].value.number = min;
            new_len++;
          }

          buffer[new_len].type = MULT;
          new_len++;
        }
      }
    }
    i++;
  }
  // There is trailing mult, so remove 1 from length
  return new_len - 1;
}
