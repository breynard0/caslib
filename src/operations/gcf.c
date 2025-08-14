#include "gcf.h"
#include "dutils.h"
#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"
#include "poly_div.h"
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

// GCF is returned in the first expression
int polynomial_gcf(struct EquationObject *expr0, int expr0_len,
                   struct EquationObject *expr1, int expr1_len) {
  // Debugging stuff
  // struct EquationObject expr0[expr0_len] = {};
  // for (int i = 0; i < expr0_len; i++) {
  //   expr0[i] = expr0a[i];
  // }
  // struct EquationObject expr1[expr1_len] = {};
  // for (int i = 0; i < expr1_len; i++) {
  //   expr1[i] = expr1a[i];
  // }

  // #include "debug.h"
  // for (int i = 0; i < expr0_len; i++) {
  //   print_eo_flat(expr0[i]);
  // }
  // for (int i = 0; i < expr1_len; i++) {
  //   print_eo_flat(expr1[i]);
  // }

  // If either is constant, then by definition, polynomial GCD is 1
  // Unless that number is zero, then it is simply whichever is not zero
  if ((expr0_len <= 2 && expr0[0].type == NUMBER) ||
      (expr1_len <= 2 && expr1[0].type == NUMBER)) {
    if (expr0[0].value.number == 0) {
      for (int i = 0; i < expr1_len; i++) {
        expr0[i] = expr1[i];
      }
      return expr1_len;
    } else if (expr1[0].value.number == 0) {
      return expr0_len;
    } else {
      expr0[0].type = NUMBER;
      expr0[0].value.number = 1.0;
      return 2;
    }
  }

  int max_len = expr0_len;
  if (expr1_len > expr0_len) {
    max_len = expr1_len;
  }

  int new_l0 = expr0_len;
  int new_l1 = expr1_len;

  Boolean expr0_larger = TRUE;

  int n = 0;

  while (new_l0 != 0 && new_l1 != 0) {
    n++;

    if (expr0[0].type == NUMBER &&
        double_abs(expr0[0].value.number) < THRESHOLD) {
      break;
    }
    if (expr1[0].type == NUMBER &&
        double_abs(expr1[0].value.number) < THRESHOLD) {
      break;
    }

    double expr0_degree = 1;
    Boolean expr0_letter_found = FALSE;
    for (int i = 0; i < new_l0; i++) {
      if (expr0[i].type == EXP) {
        expr0_degree = expr0[i + 1].value.number;
      } else if (expr0[i].type == LETTER) {
        expr0_letter_found = TRUE;
      } else if (i != 0 && (expr0[i].type == ADD || expr0[i].type == SUB)) {
        break;
      }
    }
    if (!expr0_letter_found) {
      expr0_degree = 0;
    }
    double expr1_degree = 1;
    Boolean expr1_letter_found = FALSE;
    for (int i = 0; i < new_l1; i++) {
      if (expr1[i].type == EXP) {
        expr1_degree = expr1[i + 1].value.number;
      } else if (expr1[i].type == LETTER) {
        expr1_letter_found = TRUE;
      } else if (i != 0 && (expr1[i].type == ADD || expr1[i].type == SUB)) {
        break;
      }
    }
    if (!expr0_letter_found) {
      expr0_degree = 0;
    }

    expr0_larger = expr0_degree >= expr1_degree;

    struct EquationObject quotient[8 * max_len] = {};
    struct EquationObject remainder[8 * max_len] = {};
    for (int i = 0; i < new_l0; i++) {
      if (expr0_larger) {
        quotient[i] = expr0[i];
      } else {
        remainder[i] = expr0[i];
      }
    }

    for (int i = 0; i < new_l1; i++) {
      if (expr0_larger) {
        remainder[i] = expr1[i];
      } else {
        quotient[i] = expr1[i];
      }
    }

    int quotient_len = new_l1;
    int remainder_len = new_l0;
    if (expr0_larger) {
      quotient_len = new_l0;
      remainder_len = new_l1;
    }

    polynomial_division(quotient, quotient_len, remainder, remainder_len);

    remainder_len = 0;
    while (remainder[remainder_len].type != END_LEX) {
      remainder_len++;
    }
    remainder_len++;
    remainder_len = expand_polynomial(remainder, remainder_len);

    // #include "debug.h"
    // for (int i = 0; i < remainder_len; i++) {
    //   print_eo_flat(remainder[i]);
    // }

    if (remainder_len <= 2 && remainder[0].type == NUMBER &&
        double_abs(remainder[0].value.number) < THRESHOLD) {
      remainder_len = 0;
    }

    if (expr0_larger) {
      for (int i = 0; i < remainder_len; i++) {
        expr0[i] = remainder[i];
      }

      if (remainder_len == 0 && n <= 1) {
        for (int i = 0; i < quotient_len; i++) {
          expr0[i] = quotient[i];
        }
      }
      new_l0 = remainder_len;
    } else {
      for (int i = 0; i < remainder_len; i++) {
        expr1[i] = remainder[i];
      }

      if (remainder_len == 0 && n <= 1) {
        for (int i = 0; i < quotient_len; i++) {
          expr1[i] = quotient[i];
        }
      }
      new_l1 = remainder_len;
    }
  }

  if ((expr0[0].type == NUMBER && expr0[0].value.number == 0) || new_l0 == 0) {
    for (int i = 0; i < new_l1; i++) {
      expr0[i] = expr1[i];
    }
  }

  int out_len = expand_polynomial(expr0, imaximum(new_l0, new_l1));

  // If only a constant, GCD is 1
  if (expr0[0].type == NUMBER && imaximum(new_l0, new_l1) == 2) {
    expr0[0].value.number = 1;
  }

  // Make monic
  if (expr0[0].type == NUMBER && expr0[0].value.number != 1) {
    double reciprocal = 1.0 / expr0[0].value.number;
    for (int i = 0; i < out_len; i++) {
      enum EOType last = expr0[i - 1].type;
      if (expr0[i].type == NUMBER && (i == 0 || last == ADD || last == SUB)) {
        expr0[i].value.number *= reciprocal;
      }
    }
  }
  out_len = expand_polynomial(expr0, out_len);

  return out_len;
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
