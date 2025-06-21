#include "../include/trig.h"
#include "../include/utils.h"
#include "../include/dutils.h"
#include "../include/root.h"
#include "../include/atrig.h"
#include <stdio.h>

int main() {
    // struct SinCos out = angle_pair(deg_to_rad(-315.0));
    // printf("%0.11f %0.11f\n", out.cos, out.sin);
    double num = 20000.0;
    printf("Arc function of %f: %0.10f", num, arc_tangent(num));
    return 0;
}
