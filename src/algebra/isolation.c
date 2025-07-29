#include "isolation.h"
#include "bundan.h"
#include "cauchy.h"
#include "dutils.h"
#include "equation_objects.h"
#include "expansion.h"
#include "pow.h"
#include "root.h"
#include "solve_consts.h"
#include "utils.h"

// I want 8 sig fig answers, so this'll be 9
#define THRESHOLD 0.0000000001

// Loosely based on Mignotte's separation bound
double get_small_enough(struct EquationObject *expression, int length) {
  double avg_sum = 0;
  int avg_count = 0;
  int avg_amount = 0;

  double determinate_estimate = 1.0;

  double square_sum = 0.0;

  long terms = 1;

  for (int i = 0; i < length; i++) {
    if (expression[i].type == NUMBER) {
      switch (expression[i - 1].type) {
      case NUMBER:
      case LETTER:
      case MULT:
      case DIV:
      case ADD:
      case SUB:
      case PI_VAL:
      case BLOCK_START:
      case BLOCK_END:
      case EQUAL:
      case END_LEX:
      case NONE:
        if (expression[i].value.number == 0.0) {
          avg_amount += 1;
        } else {
          determinate_estimate *= expression[i].value.number;
          avg_sum += expression[i].value.number;
          avg_count++;
        }
        square_sum += expression[i].value.number * expression[i].value.number;
        break;
      default:
        break;
      }
    }
    if (expression[i].type == ADD || expression[i].type == SUB) {
      terms++;
    }
  }

  for (int i = 0; i < avg_amount; i++) {
    determinate_estimate *= avg_sum / avg_count;
  }

  return determinate_estimate /
         (pow_ll(terms, terms / 2 + 1) *
          pow_di(square_root(square_sum), terms - 1)) /
         10;
}

struct RootRange next_delimiter(struct EquationObject *expression, int length,
                                double offset, double bound) {
  struct RootRange out;
  int starting_roots = bundan_max_roots(expression, length, offset, bound);
  if (starting_roots == 0) {
    out.min = NAN;
    out.max = NAN;
    return out;
  }

  double delim_guess = (bound - offset) / 2.0;
  double uncertainty = (bound - offset) / 2.0;

  while (uncertainty > THRESHOLD) {
    int lower_roots =
        bundan_max_roots(expression, length, delim_guess - uncertainty + offset,
                         delim_guess + offset);
    int upper_roots = bundan_max_roots(expression, length, delim_guess + offset,
                                       delim_guess + uncertainty + offset);

    uncertainty /= 2;
    if (upper_roots == 1) {
      out.min = delim_guess;
      out.max = delim_guess + 2 * uncertainty;
      return out;
    } else if (upper_roots > 1) {
      delim_guess += uncertainty;
    } else {
      delim_guess -= uncertainty;
    }
  }

  out.min = delim_guess - uncertainty;
  out.max = delim_guess + uncertainty;

  return out;
}

int get_isolation_delimiter_positions(struct EquationObject *expression,
                                      int length,
                                      struct RootRange *delimiters) {
  int out_len = 0;
  // Positive roots
  {
    double bound = get_bound_abs(expression, length);
    struct RootRange last_delim;
    while (last_delim.min == last_delim.min) {
      last_delim = next_delimiter(expression, length, 0.0, bound);
      if (last_delim.min != last_delim.min) {
        break;
      }
      delimiters[out_len] = last_delim;
      bound = last_delim.min - (THRESHOLD / 2);
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

    neg_expr_len =
        sub_letters(neg_expr, neg_expr_len, 2 * length, replace_obj, 4);
    neg_expr_len = expand_polynomial(neg_expr, neg_expr_len);

    struct RootRange last_delim;
    while (last_delim.min == last_delim.min) {
      last_delim = next_delimiter(neg_expr, neg_expr_len, 0.0, bound);
      if (last_delim.min != last_delim.min) {
        break;
      }

      double old_min = last_delim.min;
      last_delim.min = -last_delim.max;
      last_delim.max = -old_min;

      delimiters[out_len] = last_delim;
      bound = -last_delim.max - THRESHOLD;
      out_len++;
    }
  }

  return out_len;
}

int return_check_roots(struct EquationObject *expression, int length,
                       double *tentative, int tentative_len, double *out) {
  int out_len = 0;

  struct InputVar target_var;
  for (int i = 0; i < length; i++) {
    if (expression[i].type == LETTER) {
      target_var.letter = expression[i].value.letter;
    }
  }

  for (int i = 0; i < tentative_len; i++) {
    target_var.value = tentative[i];
    double thing = tentative[i];
    double comp = solve_const_expr(expression, length, &target_var, 1);
    if (double_abs(comp) <= get_small_enough(expression, length)) {
      out[out_len] = tentative[i];
      out_len++;
    }
  }

  return out_len;
}
