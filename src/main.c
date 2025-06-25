#include "../include/atrig.h"
#include "../include/dutils.h"
#include "../include/flags.h"
#include "../include/fraction.h"
#include "../include/gcf.h"
#include "../include/lex.h"
#include "../include/log.h"
#include "../include/objects.h"
#include "../include/pow.h"
#include "../include/root.h"
#include "../include/trig.h"
#include "../include/utils.h"
#include "../include/debug.h"
#include <stdio.h>

void test_lex();

int main() {
  // printf("%0.11f\n", nth_root(130130, 10));
  // struct SinCos out = angle_pair(deg_to_rad(-315.0));
  // printf("%0.11f %0.11f\n", out.cos, out.sin);
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
  // printf("%f", pow_di(2, -1));
  // printf("%f", dmodulo(8321741235453223.0324123, 8321741235453223.0));
  test_lex();
  return 0;
}

void test_lex() {
  char *expression = "3+4";
  struct EquationObject buffer[256];

  int max_len = lex(expression, 3, buffer, 256);
  
  int i = 0;
  while (i < max_len) {
    print_eo(buffer[i]);
    i++;
  }
}
