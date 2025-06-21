#include "../include/trig.h"
#include "../include/utils.h"
#include "../include/dutils.h"
#include "../include/root.h"
#include "../include/atrig.h"
#include "../include/gcf.h"
#include "../include/flags.h"
#include <stdio.h>

int main() {
    // struct SinCos out = angle_pair(deg_to_rad(-315.0));
    // printf("%0.11f %0.11f\n", out.cos, out.sin);
    // double num = 20000.0;
    // printf("Arc function of %f: %0.10f", num, arc_tangent(num));
    // struct Fraction f;
    // f.numerator = 2;
    // f.denominator = 3;
    double d = 342134124613.342134127683;
    struct Fraction f = double_to_fraction(d);
    printf(" %lli \n---, %i\n %li ", f.numerator, f_overflow, f.denominator);
    // printf("%f", pow_frac(2.0, f));
    // printf("%f", dmodulo(8321741235453223.0324123, 8321741235453223.0));
    return 0;
}
