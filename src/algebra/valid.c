#include "enums.h"
#include "equation_objects.h"

Boolean valid_expr(struct EquationObject* input, int length) {
  Boolean valid = TRUE;
  
  int i = 0;
  int block_count = 0;
  while (i < length) {
    struct EquationObject eo = input[i];
    
    if (eo.type == BLOCK_START) {
      block_count++;
    } else if (eo.type == BLOCK_END) {
      block_count--;
    }
    
    i++;
  }
  
  if (block_count != 0) {
    valid = FALSE;
  }
  
  return valid;
}
