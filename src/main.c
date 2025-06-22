#include "../include/trig.h"
#include "../include/utils.h"
#include "../include/dutils.h"
#include "../include/root.h"
#include "../include/atrig.h"
#include "../include/gcf.h"
#include "../include/flags.h"
#include "../include/log.h"
#include <stdio.h>

int main() {
  printf("%0.11f\n", nth_root(3, -10.0));
    // struct SinCos out = angle_pair(deg_to_rad(-315.0));
    // printf("%0.11f %0.11f\n", out.cos, out.sin);
    // double num = 20000.0;
    // printf("Arc function of %f: %0.10f", num, arc_tangent(num));
    // struct Fraction f;
    // f.numerator = 2;
    // f.denominator = 3;
    // double d = 0.767264;
    // struct ImproperFraction f = double_to_fraction(d);
    // printf(" %lli \n------------------, %i\n %li \n", f.numerator, f_overflow, f.denominator);
    // printf("%0.11f", d - (double)f.numerator  / (double)f.denominator);
    // printf("%f", log_n(-2102, 10));
    // printf("%f", dmodulo(8321741235453223.0324123, 8321741235453223.0));
    return 0;
}
