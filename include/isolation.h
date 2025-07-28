#include "equation_objects.h"

// Yes, this is inaccurately named. I named it this because I thought I'd have to use a proper root finding method other than this, and I might still have to so I'm keeping the name
int get_isolation_delimiter_positions(struct EquationObject* expression, int length, double* delimiters);
