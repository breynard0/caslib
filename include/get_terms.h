#include "enums.h"
#include "equation_objects.h"

struct Term {
    unsigned int numerator_length;
    unsigned int denominator_length;
    struct EquationObject data[];
};

// Returns a polynomial split into terms
int get_terms(struct EquationObject* input, int input_len, struct Term* buffer, int max_len, Boolean simplify);
