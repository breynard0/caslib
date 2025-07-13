#include "dutils.h"
#include "equation_objects.h"

double get_bound_abs(struct EquationObject *buffer, int length) {
  double leading = 1.0;
  if (buffer[0].type == NUMBER) {
    leading = buffer[0].value.number;
  }

  double largest = leading;
  for (int i = 0; i < length && buffer[i].type != END_LEX; i++) {
    if (buffer[i].type == NUMBER && buffer[i].value.number > largest) {
      if (i != 0 && buffer[i - 1].type == EXP) {
      } else {
        largest = buffer[i].value.number;
      }
    }
  }

  return double_abs(largest / leading) + 1;
}
