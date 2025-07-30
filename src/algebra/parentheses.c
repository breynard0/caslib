#include "enums.h"
#include "equation_objects.h"

int remove_redundant_parentheses(struct EquationObject *expression,
                                 int length) {
  int new_len = length;

  int i = 0;
  while (i < new_len) {
    if (expression[i].type == BLOCK_END) {
      int start = i - 1;
      int blocks = 1;
      while (blocks != 0) {
        if (expression[start].type == BLOCK_START) {
          blocks--;
        }
        if (expression[start].type == BLOCK_END) {
          blocks++;
        }
        start--;
      }
      start++;

      Boolean is_before =
          start != 0 && (expression[start - 1].type == ADD ||
                         expression[start - 1].type == SUB ||
                         expression[start - 1].type == BLOCK_START);
      Boolean is_after =
          i < new_len - 1 &&
          (expression[i + 1].type == ADD || expression[i + 1].type == SUB ||
           expression[i + 1].type == BLOCK_END);

      if ((is_before && is_after) || (start == 0 && is_after) ||
          ((i >= new_len - 1 || expression[i + 1].type == END_LEX) &&
           is_before) ||
          (start == 0 &&
           (i >= new_len - 1 || expression[i + 1].type == END_LEX))) {
        remove_eo_idx(expression, new_len, i);
        new_len--;
        remove_eo_idx(expression, new_len, start);
        new_len--;
        i -= 2;
      }
    }

    i++;
  }
  return new_len;
}
