#include "bundan.h"
#include "cauchy.h"
#include "equation_objects.h"
#include "pow.h"
#include "expansion.h"

#define THRESHOLD 0.00000000001

double next_delimiter(struct EquationObject *expression, int length,
                      double offset, double bound) {
  int starting_roots = bundan_max_roots(expression, length, offset, bound);

  double delim_guess = (bound - offset) / 2.0;
  double uncertainty = (bound - offset) / 2.0;

  while (uncertainty > THRESHOLD) {
    int lower_roots =
        bundan_max_roots(expression, length, delim_guess - uncertainty + offset,
                         delim_guess + offset);
    int upper_roots = bundan_max_roots(expression, length, delim_guess + offset,
                                       delim_guess + uncertainty + offset);

    uncertainty /= 2;
    if (upper_roots == 0) {
      delim_guess -= uncertainty;
    } else {
      delim_guess += uncertainty;
    }
  }

  return delim_guess;
}

int get_isolation_delimiter_positions(struct EquationObject *expression,
                                      int length, double *delimiters) {
  int out_len = 0;
  // Positive roots
  {
    double bound = get_bound_abs(expression, length);
    double last_delim = 10.0;
    while (last_delim > 0.0) {
      last_delim = next_delimiter(expression, length, 0.0, bound);
      delimiters[out_len] = last_delim;
      bound = last_delim - THRESHOLD;
      out_len++;
    }
  }
  // Negative roots
  {
    double bound = get_bound_abs(expression, length);
    struct EquationObject neg_expr[2 * length];
    int neg_expr_len = length;
    for (int i = 0; i < length; i++) {
      neg_expr[i] = expression[i];
    }
    
    struct EquationObject replace_obj[4];
    replace_obj[0].type = BLOCK_START;
    replace_obj[1].type = SUB;
    replace_obj[2].type = LETTER;
    for (int i = 0; i < length; i++) {
      if (expression[i].type == LETTER) {
        replace_obj[2].value.letter = expression[i].value.letter;
        break;
      }
    }
    replace_obj[3].type = BLOCK_END;
    
    neg_expr_len = sub_letters(neg_expr, neg_expr_len, 2*length, replace_obj, 4);
    neg_expr_len = expand_polynomial(neg_expr, neg_expr_len);
    
    double last_delim = 10.0;
    while (last_delim > 0.0) {
      last_delim = next_delimiter(neg_expr, length, 0.0, bound);
      delimiters[out_len] = -last_delim;
      bound = last_delim - THRESHOLD;
      out_len++;
    }
  }

  return out_len;
}
