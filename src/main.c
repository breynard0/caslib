#include "../include/trig.h"
#include "../include/utils.h"
#include "../include/futils.h"
#include <stdio.h>

int main() {
    struct SinCos out = angle_pair(deg_to_rad(90.0));
    printf("%0.11f %0.11f\n", out.cos, out.sin);
    return 0;
}
