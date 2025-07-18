#include "enums.h"
#include "equation_objects.h"

Boolean valid_expr(struct EquationObject *input, int length) {
  Boolean valid = TRUE;

  int i = 0;
  int block_count = 0;
  while (i < length) {
    struct EquationObject eo = input[i];

    switch (eo.type) {
    case MULT:
    case DIV:
    case ADD:
    case SUB:
    case EQUAL:
      if (i == length - 1 || (i == 0 && eo.type != SUB)) {
        valid = FALSE;
      } 
      break;
    case BLOCK_START:
      block_count++;
      break;
    case BLOCK_END:
      block_count--;
      break;
    case SINE:
    case COSINE:
    case TANGENT:
    case ARCSINE:
    case ARCCOSINE:
    case ARCTANGENT:
    break;
    case ROOT:
    case EXP:
    case LOG:
    if (i == 0 || i == length - 2) {
      valid = FALSE;
    }
    break;
    case END_LEX:
    case NUMBER:
    case LETTER:
    case PI_VAL:
    case NONE:
      break;
    }

    i++;
  }

  if (block_count != 0) {
    valid = FALSE;
  }

  return valid;
}

Boolean is_univariate(struct EquationObject* input, int length) {
  struct Letter letter = {};
  Boolean letter_found = FALSE;
  for (int i = 0; i < length; i++) {
    if (input[i].type == LETTER) {
      if (letter_found && (input[i].value.letter.letter != letter.letter || input[i].value.letter.subscript != letter.subscript)) {
        return FALSE;
      }
      letter_found = TRUE;
      letter = input[i].value.letter;
    }
  }
  return letter_found;
}
