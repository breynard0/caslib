#include "../include/futils.h"
#include "../include/utils.h"

float nth_root(unsigned int n, float f) {
    // Generalization of Heron/Newton's method

    // Use float exponent to use square root of close-ish poewr of 2 as first estimate
    float_cast cast = (float_cast) f;
    float estimate = 0x2 ^ ((cast.parts.exponent - 127) / 2);

    float fn = (float) n;

    // I've arbitrarily picked the number of iterations until it seems precise enough
    const int ITERATIONS = 4;
    for (int i = 0; i < ITERATIONS * n; i++) {
        estimate = (1.0 / fn) * ((fn - 1.0) * estimate + (f / pow_fi(estimate, n - 1)));
    }

    if (n % 2 == 0) {
        return float_abs(estimate);
    } else {
        return estimate;
    }
}