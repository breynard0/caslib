#include "equation_objects.h"
int get_sign_changes(struct EquationObject *expression, int length) {
  int changes = 0;
  short last_sign = 0;

  for (int i = 0; i < length; i++) {
    if (i == 0 || expression[i - 1].type == ADD ||
        expression[i - 1].type == SUB) {
      short cur_sign = 1;
      if (expression[i - 1].type == SUB) {
        cur_sign = -1;
      }
      if (expression[i].type == NUMBER && i != 0 &&
          ((expression[i].value.number < 0 && expression[i - 1].type == ADD) ||
           (expression[i].value.number > 0 && expression[i - 1].type == SUB))) {
        cur_sign = -1;
      }
      if (i == 0 && expression[i].type == NUMBER && expression[i].value.number < 0) {
        cur_sign = -1;
      }
      if (last_sign != 0 && cur_sign != last_sign) {
        changes++;
      }
      last_sign = cur_sign;
    }
  }

  return changes;
}
