#ifndef GCF_H_FRACTION
#define GCF_H_FRACTION
struct Fraction {
    long long numerator;
    long denominator;
};
#endif

long gcf(long x, long y);
void reduce_fraction(struct Fraction* fraction);
