#include "equation_objects.h"

struct Term {
    struct EquationObject* numerator;
    struct EquationObject* denominator;
};

// Returns a polynomial split into terms
int get_terms(struct EquationObject* input, int input_len, struct Term* buffer, int max_len);
