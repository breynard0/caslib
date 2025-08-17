#ifndef COLLECTION_H
#define COLLECTION_H
#include "equation_objects.h"

// Assumes all coefficients are doubles and no blocks are present
int collect_reorder_polynomial(struct EquationObject* buffer, int length);
#endif // COLLECTION_H
