#include "equation_objects.h"

#ifndef ISOLATION_STUFF
#define ISOLATION_STUFF
struct RootRange {
  double min;
  double max;
};
#endif

int get_isolation_delimiter_positions(struct EquationObject *expression,
                                      int length, struct RootRange *delimiters);
int return_check_roots(struct EquationObject *expression, int length,
                       double *tentative, int tentative_len, double *out);
