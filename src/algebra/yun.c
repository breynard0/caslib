#include "enums.h"
#include "equation_objects.h"
#include "gcf.h"
#include "derivative.h"
#include "poly_div.h"
#include "expansion.h"

// Source: Wikipedia
// I'll make this more memory efficient if I need to, I promise
int yun_decompose(struct EquationObject* expression, int length) {
  struct EquationObject a[length] = {};
  int a_len = 0;
  struct EquationObject b[length] = {};
  int b_len = length;
  struct EquationObject c[length] = {};
  int c_len = 0;
  // Making d bigger because it has to run a subtraction
  struct EquationObject d[3*length] = {};
  int d_len = length;
  
  int out_len = 0;
  
  for (int i = 0; i < length; i++) {
    b[i] = expression[i];
    d[i] = expression[i];
  }
  // Get f'
  d_len = power_rule_derivative_univariate(d, d_len);
  
  Boolean eq_1 = FALSE;
  
  while (!eq_1) {
    // Check if should exit
    if (b_len <= 2 && b[0].type == NUMBER && b[0].value.number == 1.0) {
      eq_1 = TRUE;
      break;
    }
    
    // Take GCF of b and d, store in a
    struct EquationObject b_clone[length] = {};
    int bc_len = b_len;
    for (int i = 0; i < b_len; i++) {
      b_clone[i] = b[i];
    }
    struct EquationObject d_clone[length] = {};
    for (int i = 0; i < d_len; i++) {
      d_clone[i] = d[i];
    }
    
    bc_len = polynomial_gcf(b_clone, bc_len, d_clone, d_len);
    for (int i = 0; i < bc_len; i++) {
      a[i] = b_clone[i];
    }
    a_len = bc_len;
    
    // Push a
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
    c_len = d_len;
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
  }
  
  expression[out_len].type = END_LEX;
  out_len++;
  
  return out_len;
}
