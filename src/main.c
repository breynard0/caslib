#include "atrig.h"
#include "debug.h"
#include "dutils.h"
#include "equation_objects.h"
#include "flags.h"
#include "fraction.h"
#include "gcf.h"
#include "lex.h"
#include "log.h"
#include "parse.h"
#include "pow.h"
#include "root.h"
#include "solve_consts.h"
#include "trig.h"
#include "utils.h"
#include "expansion.h"
#include <stdio.h>
#include <string.h>

void test_lex();
void test_solve_consts();
void test_expansion();

int main() {
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
  test_expansion();
  return 0;
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
  char *expression = "2*-2";
  // char *expression = "-2+2*4^2";
  // char *expression = "2^3.5";
  // char *expression = "(28\\r1600)-(100/(3^8))";
  // char *expression = "(1/(2+2))";
  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[256];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 256);
  printf("Solving...\n");
  struct InputVar var;
  var.letter.letter = 'x';
  var.letter.subscript = ' ';
  var.value = 3.0;
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
  
  // Not working still
  char *expression = "(x+1)(x+2)(x+3)(x+4)(x+5)(x+6)";
  // char *expression = "2(3x^2-4x+8(3+2))";
  
  printf("Lexing %s...\n", expression);
  struct EquationObject lex_buffer[512];
  int lex_len = lex(expression, strlen(expression), lex_buffer, 64);
  printf("Expanding...\n");
  int new_len = expand_polynomial(lex_buffer, 64);
  for (int i = 0; i < new_len; i++) {
    print_eo(lex_buffer[i]);
  }
}
