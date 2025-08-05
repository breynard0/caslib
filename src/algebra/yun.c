#include "derivative.h"
#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"
#include "gcf.h"
#include "poly_div.h"

// Source: Wikipedia
// I'll make this more memory efficient if I need to, I promise
int yun_decompose(struct EquationObject *expression, int length) {
  // Making a bigger because it has the gcd business to deal with
  struct EquationObject a[2 * length] = {};
  int a_len = 0;
  struct EquationObject b[length] = {};
  int b_len = length;
  struct EquationObject c[length] = {};
  int c_len = 0;
  // Making d bigger because it has to run a subtraction
  struct EquationObject d[3 * length] = {};
  int d_len = length;

  int out_len = 0;

  for (int i = 0; i < length; i++) {
    b[i] = expression[i];
    d[i] = expression[i];
  }
  // Get f'
  d_len = power_rule_derivative_univariate(d, d_len);

  Boolean eq_1 = FALSE;

  int n = 0;

  while (!eq_1) {
    // Check if should exit
    if (b_len <= 2 && b[0].type == NUMBER && b[0].value.number == 1.0) {
      eq_1 = TRUE;
      break;
    }

    // Take GCF of b and d, store in a
    struct EquationObject b_clone[length] = {};
    struct EquationObject old_expr[length];
    int bc_len = b_len;
    int old_expr_len = b_len;
    for (int i = 0; i < b_len; i++) {
      b_clone[i] = b[i];
      old_expr[i] = b[i];
    }
    struct EquationObject d_clone[length] = {};
    for (int i = 0; i < d_len; i++) {
      d_clone[i] = d[i];
    }

    bc_len = 0;
    while (b_clone[bc_len].type != END_LEX) {
      bc_len++;
    }
    bc_len++;
    
    d_len = 0;
    while (d_clone[d_len].type != END_LEX) {
      d_len++;
    }
    d_len++;

    bc_len = polynomial_gcf(b_clone, bc_len, d_clone, d_len);
    for (int i = 0; i < bc_len; i++) {
      a[i] = b_clone[i];
    }
    a_len = bc_len;

    // Change from monic to primitive
    // Algorithm to implement:
    // 1. Divide one of the original expressions by the GCD
    // 2. Multiply all coefficients by the first coefficient of the quotient
    // 3. Divide all of the coefficients by their GCD
    polynomial_division(old_expr, old_expr_len, b_clone, bc_len);
    double first_coeff = 1.0;
    if (old_expr[0].type == NUMBER) {
      first_coeff = old_expr[0].value.number;
    }
    struct EquationObject num_obj = {};
    num_obj.type = NUMBER;
    num_obj.value.number = first_coeff;
    struct EquationObject mult_obj = {};
    mult_obj.type = MULT;
    struct EquationObject bs_obj = {};
    bs_obj.type = BLOCK_START;
    struct EquationObject be_obj = {};
    be_obj.type = BLOCK_END;
    a_len++;
    insert_eo_idx(a, a_len, 0, num_obj);
    a_len++;
    insert_eo_idx(a, a_len, 1, mult_obj);
    a_len++;
    insert_eo_idx(a, a_len, 2, bs_obj);
    a_len++;
    insert_eo_idx(a, a_len, a_len - 2, be_obj);
    a_len = expand_polynomial(a, a_len);

    double gcf_val = 0.0;
    int count = 0;
    for (int i = 0; i < a_len; i++) {
      enum EOType last = a[i - 1].type;
      if (a[i].type == NUMBER && (i == 0 || last == ADD || last == SUB)) {
        gcf_val = gcf(a[i].value.number, gcf_val);
        count++;
      }
    }

    if (count > 1) {
      double reciprocal_gcf = 1.0 / gcf_val;

      num_obj.value.number = reciprocal_gcf;
      a_len++;
      insert_eo_idx(a, a_len, 0, num_obj);
      a_len++;
      insert_eo_idx(a, a_len, 1, mult_obj);
      a_len++;
      insert_eo_idx(a, a_len, 2, bs_obj);
      a_len++;
      insert_eo_idx(a, a_len, a_len - 2, be_obj);
      a_len = expand_polynomial(a, a_len);
    }

    // Push a, so long as it isn't a single number 1 and it isn't a0
    if ((a[0].type != NUMBER || a[0].value.number != 1) && n > 0) {
      int i = 0;
      expression[out_len].type = BLOCK_START;
      out_len++;
      while (a[i].type != END_LEX) {
        expression[out_len] = a[i];
        out_len++;
        i++;
      }
      expression[out_len].type = BLOCK_END;
      out_len++;
    }

    // Set b and c to the divisions
    struct EquationObject a_clone[length] = {};
    for (int i = 0; i < a_len; i++) {
      a_clone[i] = a[i];
    }
    polynomial_division(b, b_len, a, a_len);
    b_len = 0;
    while (b[b_len].type != END_LEX) {
      b_len++;
    }
    b_len++;

    for (int i = 0; i < d_len; i++) {
      c[i] = d[i];
    }
    c_len = 0;
    while (c[c_len].type != END_LEX) {
      c_len++;
    }
    polynomial_division(c, c_len, a_clone, a_len);
    c_len = 0;
    while (c[c_len].type != END_LEX) {
      c_len++;
    }

    // Get derivative of b and then run subtraction in d
    struct EquationObject b_d[length] = {};
    int b_d_len = b_len;
    for (int i = 0; i < b_len; i++) {
      b_d[i] = b[i];
    }
    b_d_len = power_rule_derivative_univariate(b_d, b_d_len) - 1;

    int d_len = 0;
    d[d_len].type = BLOCK_START;
    d_len++;
    for (int i = 0; i < c_len; i++) {
      d[d_len] = c[i];
      d_len++;
    }
    d[d_len].type = BLOCK_END;
    d_len++;
    d[d_len].type = SUB;
    d_len++;
    d[d_len].type = BLOCK_START;
    d_len++;
    for (int i = 0; i < b_d_len; i++) {
      d[d_len] = b_d[i];
      d_len++;
    }
    d[d_len].type = BLOCK_END;
    d_len++;
    d[d_len].type = END_LEX;
    d_len++;
    d_len = expand_polynomial(d, d_len);
    d_len = 0;
    while (d[d_len].type != END_LEX) {
      d_len++;
    }
    n++;
  }

  expression[out_len].type = END_LEX;
  out_len++;

  return out_len;
}
