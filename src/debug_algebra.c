#include "atrig.h"
#include "bisection.h"
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
#include "poly_div.h"
#include "pow.h"
#include "rearrange.h"
#include "root.h"
#include "solve_consts.h"
#include "trig.h"
#include "utils.h"
#include "valid.h"
#include "yun.h"
#include <stdio.h>
#include <string.h>

void test_lex();
void test_solve_consts();
void test_expansion();
void test_roots();
void test_roots_function();
void test_valid();
void test_rearrange();
void test_derivative();
void test_polydiv();
void test_polygcf();
void test_yun();

void debug_algebra() {
  // printf("%0.11f\n", nth_root(10000, 1000000));
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
  // double d = 1.33333333333333567;
  // struct MixedFraction f = double_to_mixed_fraction(d);
  // struct ImproperFraction f = double_to_fraction(d);
  // printf("%f to mixed fraction...\n", d);
  // printf("     %lli \n%li -----\n     %li \n", f.numerator, f.integer,
  //        f.denominator);
  // printf(" %lli \n------------------, %li\n %li \n",
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
  // test_roots_function();
  // test_valid();
  // printf("Test rearrange...\n");
  // test_rearrange();
  // printf("Test power rule...\n");
  // test_derivative();
  // test_polydiv();
  // test_polygcf();
  // test_yun();
}

void test_lex() {
  // char *expression =
  //     "(22+a:2)*23.12/\\r2\\4=-(0.6^9-e^(-2\\p)+\\g(0.3x:6)\\l26\\";
  // char *expression = "-2+2*4^2";
  // char *expression = "4xy";
  // char *expression = "60\\d";
  // char *expression = "3x+4";
  // char *expression = "(3(x-4)^2+10)(10(x-1)^2+2)";
  char *expression = "3";
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
  // char *expression = "x^2+x-6";
  // char *expression = "(x-2)(x-2)(x-2)(x-2)(x-2)(x-3)";
  char *expression = "\\s\\s40";
  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[256];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 256);
  printf("Solving...\n");
  struct InputVar var;
  var.letter.letter = 'x';
  var.letter.subscript = ' ';
  var.value = 2;
  struct InputVar vars[1] = {var};
  double solution = solve_const_expr(lex_buffer, lex_len, vars, 1);
  printf("%0.12f", solution);
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
  // char *expression = "3*(x+2)^3";
  // char *expression = "-1*(((x^2+4)))";
  // char *expression = "(x^2+4)-(x^2-3x)";
  // char *expression = "(x^2+4)-(x)(x-3)";
  // char *expression = "(3x^3+4x^2-x+9)-(3x)(x^2-3x+4)";
  // char *expression = "3x^3+4x^2-x+9-x^2*3*x+3x*3x-4*3x";
  // char *expression = "3x^3+4x^2-x+9-3x^3+9x^2-4*3x";
  // char *expression = "2^(2^-1)";
  // char *expression = "(13x^2-13x+9)-(13x^2-39x+52)";
  // char *expression = "(13x^2-13x+9)-(13)(x^2-3x+4)";
  // char *expression = "(x^3)-(x)(x^2)";
  // char *expression = "1/1";
  // char *expression = "x^3+1.0000000000000314x^2-12.999999999999997x+13."
  //                    "9999999999999999999999999999999999934";
  // char *expression = "(x-2)(x-2)(x-2)(x-2)(x-2)(x-2)(x-2)(x-2)(3x+4)";
  // char *expression =
  // "0.000000*x^6.000000-0.000000*x^5.000000+0.000000*x^4.000000-0.000000*x^3.000000-0.000000*x^2.000000+0.000000*x-0.000000";
  // char *expression = "3x-8x";
  // char *expression = "2/0";
  // char *expression = "(3(x-4)^2+10)(10(x-1)^2+2)";
  // char *expression = "((x((x-1))-4((x-1))))";
  // char *expression = "(3(x-4))(10(x-1))";
  char *expression = "(3(x-4))(10(x-1))";

  // Not working still

  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[1024];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 64);
  printf("Expanding...\n");
  int new_len = expand_polynomial(lex_buffer, 1024);
  for (int i = 0; i < new_len; i++) {
    print_eo_flat(lex_buffer[i]);
  }
}

void test_rearrange() {
  // char *expression = "d=vt-(1/2)at^2";
  // char *expression = "12axy^3+yz=4xy^2+c";
  // char *expression = "x+4=5";
  char *expression = "5x+10=20y";

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
  // char *expression = "(x+2)(x-3)(x^3-18)";
  // char *expression = "x^5-x^4-6x^3-18x^2+18x+108";
  // char *expression = "(x-2)(x-3)(x-5)(x-7)(x-8)(x-36)";
  // char *expression = "(x+200)(x+3)(x-50)(x-70)(x-80)(x-3600)";
  // char *expression = "(x-2)(x-2)(x-2)(x-2)(x-2)(x-1)";
  // char *expression = "(x-2)(x+3)(x-3)";
  // char *expression = "(x-2)(x-2.05)";
  // char *expression = "(x-2)(x-3)";
  // char *expression = "x^2-5x+6";
  // char *expression =
  // "3x^9-44x^8+272x^7-896x^6+1568x^5-896*x^4-1792x^3+4096x^2-3328x+1024"; char
  // *expression = "(x-2)(x-2)(x-3)(x-3)(x^2-128)"; char *expression =
  // "x^6-10x^5-91x^4+1220x^3-4700x^2+7680x-4608"; char *expression =
  // "((x-2)(x-2)(x-3)(x-3)(x^2-128))/(3x+4)"; char *expression =
  // "(3(x-4)^2+10)(10(x-1)^2+2)";
  // char *expression = "(3(x-4)^2+10)(10(x-1)^2+2)";
  // char *expression = "x+1";
  char *expression = "x^2+1";
  // char *expression = "x^2-1";

  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[1024];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 1024);
  printf("Expanding...\n");
  int new_len = expand_polynomial(lex_buffer, 1024);
  printf("Expanded: ");
  if (f_unsupported == 1) {
    printf("Unsupported\n");
  }
  for (int i = 0; i < new_len; i++) {
    print_eo_flat(lex_buffer[i]);
  }

  printf("Removing squares...\n");
  new_len = yun_decompose(lex_buffer, new_len);
  printf("Square free decomposition: ");
  for (int i = 0; i < new_len; i++) {
    print_eo_flat(lex_buffer[i]);
  }
  new_len = expand_polynomial(lex_buffer, new_len);
  printf("Square free polynomial: ");
  for (int i = 0; i < new_len; i++) {
    print_eo_flat(lex_buffer[i]);
  }

  printf("Calculating bound...\n");
  double bound_abs = get_bound_abs(lex_buffer, new_len);
  printf("Bound calculated: %f < x < %f\n", -bound_abs, bound_abs);
  // int roots = bundan_max_roots(lex_buffer, new_len, 0.0, bound_abs);
  // printf("Max positive roots: %i\n", roots);
  printf("Solving...\n");
  struct RootRange delimiters[new_len] = {};
  int out_delim_len =
      get_isolation_delimiter_positions(lex_buffer, new_len, delimiters);
  for (int i = 0; i < out_delim_len; i++) {
    printf("Root interval: %0.8f-%0.8f\n", delimiters[i].min,
           delimiters[i].max);
    double solution = get_root_bisection(delimiters[i], lex_buffer, new_len);
    printf("Solution: %0.8f\n", solution);
  }
}

void test_roots_function() {
  // char *expression = "(3(x-4)^2+10)(10(x-1)^2+2)";
  // char *expression = "(x-3)(x+2)(7x-2)";
  char *expression = "3x+4";

  struct EquationObject lex_buffer[1024];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 1024);
  int new_len = expand_polynomial(lex_buffer, 1024);

  double out_roots[64] = {};

  int num_roots = real_roots(lex_buffer, new_len, out_roots);
  for (int i = 0; i < num_roots; i++) {
    printf("%0.16f\n", out_roots[i]);
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
  // char *expression = "x^4-x^3+x^2+x-4";
  // char *expression = "x^2+5x+6";
  char *expression = "x+2";

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

void test_polydiv() {
  // char *dividend = "3x^3+4x^2-x+9";
  // char *divisor = "x^2-3x+4";

  // char *dividend = "2x^4+3x^3+3x^2-5x-3";
  // char *divisor = "2x^2-x-1";

  // char *dividend = "-x^5+7x^3-x";
  // char *divisor = "x^3-x^2+1";

  // char *dividend = "9x^2+3x-6";
  // char *divisor = "3";

  // char *dividend = "x^2-3x+2";
  // char *divisor = "6x-12";

  // char *dividend = "2x+3";
  // char *divisor = "3x+2";

  // char *dividend = "x^4-1";
  // char *divisor = "x^3+x^2+x+1";

  // char *dividend = "x^3";
  // char *divisor = "x^2";

  // char *dividend = "15x^2+10x";
  // char *divisor = "9x+6";

  // char *dividend = "3x+4";
  // char *divisor = "3x+4";

  // char *dividend = "x^5-87x^4+9x^3+7x^2-3";
  // char *divisor = "-1207.44x^3+98.16x^2+48.72x-3";

  // char *dividend = "x^5-87x^4+9x^3+7x^2-3";
  // char *divisor = "-1207.44x^3+98.16x^2+48.72x-3";
  
  char *dividend = "2x";
  char *divisor = "1";

  printf("Lexing %s...\n", dividend);
  struct EquationObject dividend_buffer[1024];
  int dividend_len = lex(dividend, strlen(dividend), dividend_buffer, 64);
  printf("Lexing %s...\n", divisor);
  struct EquationObject divisor_buffer[1024];
  int divisor_len = lex(divisor, strlen(divisor), divisor_buffer, 64);
  printf("Expanding...\n");
  dividend_len = expand_polynomial(dividend_buffer, dividend_len);
  divisor_len = expand_polynomial(divisor_buffer, divisor_len);
  printf("Dividing...\n");
  polynomial_division(dividend_buffer, dividend_len, divisor_buffer,
                      divisor_len);
  printf("Quotient: ");
  for (int i = 0; dividend_buffer[i - 1].type != END_LEX; i++) {
    print_eo_flat(dividend_buffer[i]);
  }
  printf("Remainder: ");
  for (int i = 0; divisor_buffer[i - 1].type != END_LEX; i++) {
    print_eo_flat(divisor_buffer[i]);
  }
}

void test_polygcf() {
  // char *dividend = "3x^3+4x^2-x+9";
  // char *divisor = "x^2-3x+4";

  // char *dividend = "2x^4+3x^3+3x^2-5x-3";
  // char *divisor = "2x^2-x-1";

  // char *dividend = "-x^5+7x^3-x";
  // char *divisor = "x^3-x^2+1";

  // char *dividend = "x^5-2x^4+x^2-x-2";
  // char *divisor = "x^3-x^2-x-2";

  // char *dividend = "(x-2)(x-4)(x-3)";
  // char *divisor = "(x-2)(x-1)";

  // char *dividend = "x^4-1";
  // char *divisor = "x^3+x^2+x+1";

  // char *dividend = "0";
  // char *divisor = "x^2+1";

  // char *dividend = "x^3";
  // char *divisor = "x^2";

  // char *dividend = "5";
  // char *divisor = "7";

  // char *dividend = "6x^3+18x^2+12x";
  // char *divisor = "4x^2+8x";

  // char *dividend = "15x^2+10x";
  // char *divisor = "9x+6";

  // char *dividend = "x^3+7x^2+16x+12";
  // char *divisor = "3x^2+14x+16";

  // char *dividend =
  // "x^3+1.0000000000000314x^2-12.999999999999997x+13.99999999999934"; char
  // *divisor = "x^2+3.00000000000001x-6.9999999999999999999";

  // char *dividend = "3x^3-8x^2-4x+16";
  // char *divisor = "9x^2-16x-4";

  // char *dividend = "0";
  // char *divisor = "4x+5";

  // char *dividend =
  //     "3x^9-44x^8+272x^7-896x^6+1568x^5-896x^4-1792x^3+4096x^2-3328x+1024";
  // char *divisor =
  //     "27x^8-352x^7+1904x^6-5376x^5+7840x^4-3584x^3-5376x^2+8192x-3328";

  // char *dividend = "x^5-87x^4+9x^3+7x^2-3";
  // char *divisor = "5x^4-348x^3+27x^2+14x";

  char *dividend = "x^2+1";
  char *divisor = "2x";

  printf("Lexing %s...\n", dividend);
  struct EquationObject dividend_buffer[1024];
  int dividend_len = lex(dividend, strlen(dividend), dividend_buffer, 64);
  printf("Lexing %s...\n", divisor);
  struct EquationObject divisor_buffer[1024];
  int divisor_len = lex(divisor, strlen(divisor), divisor_buffer, 64);
  printf("Expanding...\n");
  dividend_len = expand_polynomial(dividend_buffer, dividend_len);
  divisor_len = expand_polynomial(divisor_buffer, divisor_len);
  printf("Getting GCF...\n");
  int out_len = polynomial_gcf(dividend_buffer, dividend_len, divisor_buffer,
                               divisor_len);
  printf("GCF: ");
  for (int i = 0; i < out_len; i++) {
    print_eo_flat(dividend_buffer[i]);
  }
}

void test_yun() {
  // char *expression = "x^3+7x^2+16x+12";
  // char *expression = "x^5-87x^4+9x^3+7x^2-3";
  // char *expression = "x^4-x^3-15x^2+40x-28";
  // char *expression = "(x-2)(x-2)(x-2)(x-2)(x-2)(x-2)(x-2)(x-2)(3x+4)";
  char *expression = "x^5-14x^4+77x^3-208x^2+276x-144";
  // char *expression =
  // "3x^9-44x^8+272x^7-896x^6+1568x^5-896*x^4-1792x^3+4096x^2-3328x+1024"; char
  // *expression = "(x-2)(x-2)(3x+4)"; char *expression = "3x^3-8x^2-4x+16";

  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[1024];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 64);
  printf("Decomposing...\n");
  int new_len = expand_polynomial(lex_buffer, 1024);
  new_len = yun_decompose(lex_buffer, new_len);

  printf("Square free polynomial: ");
  for (int i = 0; i < new_len; i++) {
    print_eo_flat(lex_buffer[i]);
  }
}
