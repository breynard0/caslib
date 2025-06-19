#include "../include/dutils.h"
#include "../include/utils.h"

double nth_root(unsigned int n, double d) {
    // Generalization of Heron/Newton's method

    // Use double exponent to use square root of close-ish poewr of 2 as first estimate
    double_cast cast = (double_cast) d;
    double estimate = 0x2 ^ ((cast.parts.exponent - 1023) / 2);

    double fn = (double) n;

    // I've arbitrarily picked the number of iterations until it seems precise enough
    const int ITERATIONS = 4;
    for (int i = 0; i < ITERATIONS * n; i++) {
        estimate = (1.0 / fn) * ((fn - 1.0) * estimate + (d / pow_di(estimate, n - 1)));
    }

    if (n % 2 == 0) {
        return double_abs(estimate);
    } else {
        return estimate;
    }
}
