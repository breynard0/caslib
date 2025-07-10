#include "equation_objects.h"

int cull_the_useless(struct EquationObject *expression, int length) {
  int new_len = length;
  int i = 1;
  while (i < new_len) {
    // x^0
    if (expression[i].type == NUMBER && expression[i].value.number == 0 &&
        expression[i - 1].type == EXP && expression[i - 2].type == LETTER) {
      for (int j = 0; j < 4; j++) {
        remove_eo_idx(expression, new_len, i - 3);
        new_len--;
      }
    }
    // 1*
    if ((expression[i].type == NUMBER && expression[i].value.number == 1 &&
         expression[i - 1].type == MULT) ||
        (expression[i].type == MULT && expression[i - 1].type == NUMBER &&
         expression[i - 1].value.number == 1)) {
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
    }
    // 0+
    // -0 should be filtered, 0- should not
    if ((expression[i].type == NUMBER && expression[i].value.number == 0 &&
         (expression[i - 1].type == ADD || expression[i - 1].type == SUB)) ||
        (expression[i].type == ADD && expression[i - 1].type == NUMBER &&
         expression[i - 1].value.number == 0)) {
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
    }
    i++;
  }

  return new_len;
}
