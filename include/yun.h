#include "equation_objects.h"

// Decomposition is likely larger than expanded form, make sure there's enough
// space Does NOT return number of multiplicities, only what they are. For the
// purposes of this project, that is enough.
int yun_decompose(struct EquationObject* expression, int length);