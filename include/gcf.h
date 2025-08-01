#include "equation_objects.h"
#include "fraction.h"

long gcf(long x, long y);
// GCF is returned in the first expression
int polynomial_gcf(struct EquationObject* expr0, int expr0_len, struct EquationObject* expr1, int expr1_len);
void reduce_fraction(struct ImproperFraction *fraction);
int term_gcf(struct EquationObject* term0, int t0_len, struct EquationObject* term1, int t1_len, struct EquationObject* buffer, int max_len, struct Letter exclude);
