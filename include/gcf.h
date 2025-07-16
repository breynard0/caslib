#include "equation_objects.h"
#include "fraction.h"

long gcf(long x, long y);
void reduce_fraction(struct ImproperFraction *fraction);
int term_gcf(struct EquationObject* term0, int t0_len, struct EquationObject* term1, int t1_len, struct EquationObject* buffer, int max_len);
