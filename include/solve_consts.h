#include "equation_objects.h"
#include "parse.h"

// Evaluates an expression with all values known
// buffer must be only letter vars
double solve_const_expr(struct EquationObject *input, int length,
                        struct InputVar *buffer, int num_args);
