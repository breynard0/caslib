#include "enums.h"
#include "equation_objects.h"

struct Term {
  unsigned int numerator_length;
  unsigned int denominator_length;
  struct EquationObject *data[];
};

// Returns a polynomial split into terms
int get_terms(struct EquationObject *input, int input_len,
              struct Term *term_buf, int term_max_len,
              struct EquationObject *data_buf, int data_max_len,
              Boolean simplify);
