#include "../include/trig.h"
#include "../include/utils.h"
#include "../include/dutils.h"
#include "../include/root.h"
#include "../include/atrig.h"
#include <stdio.h>

int main() {
    // struct SinCos out = angle_pair(deg_to_rad(-315.0));
    // printf("%0.11f %0.11f\n", out.cos, out.sin);
    // printf("%0.11f\n", nth_root(2,0.0000000000000000001));
    printf("%0.10f", arc_sine(0.999));
    return 0;
}
