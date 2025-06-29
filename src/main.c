#include "../include/atrig.h"
#include "../include/debug.h"
#include "../include/dutils.h"
#include "../include/equation_objects.h"
#include "../include/flags.h"
#include "../include/fraction.h"
#include "../include/gcf.h"
#include "../include/lex.h"
#include "../include/log.h"
#include "../include/parse.h"
#include "../include/pow.h"
#include "../include/root.h"
#include "../include/solve_consts.h"
#include "../include/trig.h"
#include "../include/utils.h"
#include <stdio.h>
#include <string.h>

void test_lex();
void test_solve_consts();

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
  printf("%0.16f", pow_dd(2, 33.82314132412341256));
  // printf("%0.10f\n", log_n(1600, 10));
  // printf("%0.10f\n", log_2(160000));
  // printf("%0.10f\n", ln(200000));
  // printf("%f", dmodulo(8321741235453223.0324123, 8321741235453223.0));
  // test_lex();
  // test_solve_consts();
  return 0;
}

void test_lex() {
  // char *expression =
  //     "(22+a:2)*23.12/\\r2\\4=-(0.6^9-e^(-2\\p)+\\g(0.3x:6)\\l26\\";
  char *expression = "-2+2*4^2";
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
  // char *expression = "-2(2+3)+2*4x-(-2)3";
  // char *expression = "6+5*2-10/2";
  // char *expression = "6-5*5-72/2";
  // char *expression = "2*(1+32)";
  // char *expression = "2-(-2)";
  // char *expression = "-2+2*4^2";
  char *expression = "2^3.5";
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
