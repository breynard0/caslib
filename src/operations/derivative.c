#include "equation_objects.h"
#include "enums.h"

int power_rule_derivative(struct EquationObject* function, int length) {
  int new_len = length;
  
  int i = 0;
  while (i < new_len)  {
    if (function[i].type == ADD || function[i].type == SUB || i == length - 1) {
      int start = i - 1;
      while (function[start].type != ADD && function[i].type != SUB && i > 0) {
        start--;
      }
      if (function[start].type == ADD || function[i].type == SUB) {
        start++;
      }
      
      // Get n and reduce exponent
      double n = 1.0;
      Boolean found = FALSE;
      for (int j = 0; j < i - start; j++) {
        if (function[start + j].type == EXP) {
          found = TRUE;
          n = function[start + j + 1].value.number;
          function[start + j + 1].value.number -= 1.0;
          break;
        }
      }
      if (n == 1.0 && !found) {
        int letter_idx = start;
        while (function[letter_idx].type != LETTER) {
          letter_idx++;
        }
        // new_len++;
        // insert_eo_idx(function, new_len, letter_idx + 1, zero_obj);
        // new_len++;
        // insert_eo_idx(function, new_len, letter_idx + 1, exp_obj);
      }
    }
    i++;
  }
  
  return new_len;
}
