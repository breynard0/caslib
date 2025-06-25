#ifndef GCF_H_FRACTIONS
#define GCF_H_FRACTIONS
struct ImproperFraction {
  long long numerator;
  long denominator;
};
struct MixedFraction {
  long integer;
  long long numerator;
  long denominator;
};
#endif

struct ImproperFraction double_to_fraction(double num);
struct MixedFraction double_to_mixed_fraction(double num);
