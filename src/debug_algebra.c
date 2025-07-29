#include "atrig.h"
#include "bundan.h"
#include "cauchy.h"
#include "debug.h"
#include "derivative.h"
#include "dutils.h"
#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"
#include "flags.h"
#include "fraction.h"
#include "gcf.h"
#include "isolation.h"
#include "lex.h"
#include "log.h"
#include "parse.h"
#include "pow.h"
#include "rearrange.h"
#include "root.h"
#include "solve_consts.h"
#include "trig.h"
#include "utils.h"
#include "valid.h"
#include <stdio.h>
#include <string.h>

void test_lex();
void test_solve_consts();
void test_expansion();
void test_roots();
void test_valid();
void test_rearrange();
void test_derivative();

void debug_algebra() {
  // printf("%0.11f\n", nth_root(130130, 10));
  // struct SinCos out = angle_pair(deg_to_rad(-315.0));
  // printf("%0.11f %0.11f\n", out.cos, out.sin);
  // printf("%0.11f\n", sine(cosine(tangent(43))));
  // double num = 20000.0;
  // printf("Arc function of %f: %0.10f", num, arc_tangent(num));
  // printf("%li", gcf(-10, -15));
  // struct ImproperFraction f;
  // f.numerator = 6;
  // f.denominator = 10;
  // reduce_fraction(&f);
  // double d = 1.137;
  // struct MixedFraction f = double_to_mixed_fraction(d);
  // struct ImproperFraction f = double_to_fraction(d);
  // printf("%f to mixed fraction...\n", d);
  // printf("     %lli \n%li -----\n     %li \n", f.numerator, f.integer,
  // f.denominator); printf(" %lli \n------------------, %li\n %li \n",
  // f.numerator, 0, f.denominator); printf("%0.11f", d - (double)f.numerator  /
  // (double)f.denominator); printf("%0.10f", log_n(16, 10)); printf("%f",
  // pow_frac(10.0, f)); printf("%f", pow_dd(2.0, 4.3)); printf("%f", pow_di(3,
  // 8));
  // printf("%i", double_digits_partial(0.123456789));
  // printf("%f", dfloor(12232132998.000001534820));
  // printf("%f", dround(1999.99));
  // printf("%f", pow_di(2, -1));
  // printf("%0.16f", pow_dd(2, 33.82314132412341256));
  // printf("%0.10f\n", log_n(1600, 10));
  // printf("%0.10f\n", log_2(160000));
  // printf("%0.10f\n", ln(200000));
  // printf("%f", dmodulo(8321741235453223.0324123, 8321741235453223.0));
  // test_lex();
  // test_solve_consts();
  // test_expansion();
  test_roots();
  // test_valid();
  // printf("Test rearrange...\n");
  // test_rearrange();
  // printf("Test power rule...\n");
  // test_derivative();
}

void test_lex() {
  // char *expression =
  //     "(22+a:2)*23.12/\\r2\\4=-(0.6^9-e^(-2\\p)+\\g(0.3x:6)\\l26\\";
  // char *expression = "-2+2*4^2";
  char *expression = "4xy";
  printf("Lexing %s\n", expression);
  struct EquationObject buffer[256];

  int max_len = lex(expression, strlen(expression), buffer, 256);

  printf("---------------------------------\n");

  int i = 0;
  while (i < max_len) {
    print_eo(buffer[i]);
    i++;
  }
}

void test_solve_consts() {
  // char *expression = "-2(2+3)+2*4x-(-2)3*10\\r29";
  // char *expression = "6+5*2-10/2";
  // char *expression = "6-5*5-72/2";
  // char *expression = "2(1+(-32))";
  // char *expression = "2*-2";
  // char *expression = "-2+2*4^2";
  // char *expression = "2^3.5";
  // char *expression = "(28\\r1600)-(100/(3^8))";
  // char *expression = "(1/(2+2))";
  char *expression = "x^2+x-6";
  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[256];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 256);
  printf("Solving...\n");
  struct InputVar var;
  var.letter.letter = 'x';
  var.letter.subscript = ' ';
  var.value = -3.0;
  struct InputVar vars[1] = {var};
  double solution = solve_const_expr(lex_buffer, lex_len, vars, 1);
  printf("%0.10f", solution);
}

void test_expansion() {
  // Working
  // char *expression = "x^2*3*2*x+7*8*x/2";
  // char *expression = "4x^2+3";
  // char *expression = "4(3+x)";
  // char *expression = "4x^2.3*x^1.7";
  // char *expression = "3x-2x-x";
  // char *expression = "3x+4+5x^2";
  // char *expression = "3x+4x+5x";
  // char *expression = "3x+2x^2+4x+7x^2";
  // char *expression = "3x+2x^2+4x+5x+7x^2";
  // char *expression = "4x^2-3+3x^2+7x-2x+2";
  // char *expression = "4x+3y+7x";
  // char *expression = "4xy";
  // char *expression = "4x^2yz^8+3x^2yz^8+7x";
  // char *expression = "2+3+4";
  // char *expression = "3x^2+2x-6x";
  // char *expression = "x(x+4)+3(x+4)";
  // char *expression = "(x+3)2";
  // char *expression = "(x*3)";
  // char *expression = "x+3+x+4";
  // char *expression = "((()()()()((x+3)+(x+4)())())())";
  // char *expression = "(x+3)+(x+4)2";
  // char *expression = "((()()()()((x+3)+(x+4)2())())())";
  // char *expression = "2*(x+1)(x+2)";
  // char *expression = "(x+1)(x+2)2";
  // char *expression = "(x+1)(x+2)(x+3)";
  // char *expression = "(x+1)(x+2)(x+3)(x+4)(x+5)(x+6)";
  // char *expression = "2(x+2)(x+2)+5";
  // char *expression = "(x+2)^40";
  // char *expression = "x*(x+1)+(x+1)";
  // char *expression = "(x+1)(x+1)";
  // char *expression = "(x+2)(x+y)";
  // char *expression = "(x+2)(x-2)";
  // char *expression = "(x-1)^6";
  // char *expression = "2(3x^2-4x+8(3+2))";
  // char *expression = "3/2";
  // char *expression = "3+x";
  // char *expression = "x:1+3";
  // char *expression = "xyz/(3xz)";
  // char *expression = "3+8y";
  // char *expression = "10abc/(5def)";
  // char *expression = "(4x^3)/(x^8)";
  // char *expression = "(4x^3+2x^2)/(2x^2)";
  // char *expression = "-(4x^3+2x^2-32x+9)";
  char *expression = "3*(x+2)^3";

  // Not working still
  // char *expression = "2^(2^-1)";

  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[1024];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 64);
  printf("Expanding...\n");
  int new_len = expand_polynomial(lex_buffer, 1024);
  for (int i = 0; i < new_len; i++) {
    print_eo(lex_buffer[i]);
  }
}

void test_rearrange() {
  // char *expression = "d=vt-(1/2)at^2";
  char *expression = "12axy^3+yz=4xy^2+c";
  // char *expression = "x+4=5";

  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[1024];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 64);
  printf("Rearranging...\n");
  struct Letter target;
  target.letter = 'x';
  target.subscript = ' ';
  int end_len = rearrange_for_var(lex_buffer, lex_len, target);
  for (int i = 0; i < end_len; i++) {
    print_eo_flat(lex_buffer[i]);
  }
}

void test_roots() {
  // TODO: amend algorithm if division by polynomial
  // Frankly, can probably just ignore the denominator. Multiply both sides by
  // the polynomial, but other side is zero

  // char *expression = "3/2+(3x/3)/(9x-1)-2";
  // char *expression = "(x+2)(x-3)(x^3-8)";
  // char *expression = "x^5-x^4-6x^3-18x^2+18x+108";
  // char *expression = "(x-2)(x-3)(x-5)(x-7)(x-8)(x-36)";
  char *expression = "(x-2)(x-2)(x-2)(x-2)(x-2)(x-3)";
  // char *expression = "(x-2)(x+3)(x-3)";
  // char *expression = "(x-2)(x-2.05)";

  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[1024];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 64);
  printf("Simplifying...\n");
  int new_len = expand_polynomial(lex_buffer, 1024);
  printf("Simplified: ");
  for (int i = 0; i < new_len; i++) {
    print_eo_flat(lex_buffer[i]);
  }
  printf("Calculating bound...\n");
  double bound_abs = get_bound_abs(lex_buffer, new_len);
  printf("Bound calculated: %f < x < %f\n", -bound_abs, bound_abs);
  // printf("Getting number of roots...\n");
  // int sign_changes = get_sign_changes(lex_buffer, new_len);
  // printf("Sign changes: %i\n", sign_changes);
  // 2.00086212-2.00331116
  // double l = 2.00086212;
  // double r = 2.00331116;
  // printf("Getting roots between %f and %f\n", l, r);
  // int roots = bundan_max_roots(lex_buffer, new_len, l, r);
  int roots = bundan_max_roots(lex_buffer, new_len, 0.0, bound_abs);
  printf("Max roots: %i\n", roots);
  printf("Solving...\n");
  struct RootRange delimiters[new_len] = {};
  int out_delim_len =
      get_isolation_delimiter_positions(lex_buffer, new_len, delimiters);
  // double out[out_delim_len] = {};
  // int final_len =
  //     return_check_roots(lex_buffer, new_len, delimiters, out_delim_len, out);
  for (int i = 0; i < out_delim_len; i++) {
    printf("Root interval: %0.8f-%0.8f\n", delimiters[i].min, delimiters[i].max);
  }
}

void test_valid() {
  char *expression = "z+/x";
  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[1024];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 64);
  printf("Checking...\n");
  Boolean out = valid_expr(lex_buffer, lex_len);
  // Boolean out = is_univariate(lex_buffer, lex_len);
  if (out) {
    printf("True");
  } else {
    printf("False");
  }
}

void test_derivative() {
  // char *expression = "3x^2-4x+9";
  // char *expression = "78x^4-2x^3+6x^2+8x-4";
  char *expression = "x^4-x^3+x^2+x-4";

  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[1024];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 64);
  printf("Calculating derivative...\n");
  int new_len = expand_polynomial(lex_buffer, 1024);
  new_len = power_rule_derivative_univariate(lex_buffer, new_len);
  for (int i = 0; i < new_len; i++) {
    print_eo_flat(lex_buffer[i]);
  }
}
