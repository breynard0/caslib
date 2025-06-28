#include "../../include/solve_consts.h"
#include "../../include/debug.h"
#include "../../include/enums.h"
#include "../../include/equation_objects.h"
#include "../../include/flags.h"
#include "../../include/parse.h"
#include "../../include/root.h"
#include "../../include/utils.h"

static Boolean is_function(enum EOType type) {
  switch (type) {
  case PI_VAL:
  case ROOT:
  case EXP:
  case SINE:
  case COSINE:
  case TANGENT:
  case ARCSINE:
  case ARCCOSINE:
  case ARCTANGENT:
  case LOG:
    return TRUE;
  default:
    return FALSE;
  }
}

static Boolean is_juxtaposed(struct EquationObject self,
                             struct EquationObject last) {
  // (2)3
  if (self.type == NUMBER && last.type == BLOCK_END) {
    return TRUE;
  }
  // 2(2+3)
  if (self.type == BLOCK_START && last.type == NUMBER) {
    return TRUE;
  }
  // ()
  if (self.type == BLOCK_END && last.type == BLOCK_START) {
    return TRUE;
  }
  // 5x
  if (self.type == LETTER && last.type == NUMBER) {
    return TRUE;
  }
  // (3)x
  if (self.type == LETTER && last.type == BLOCK_END) {
    return TRUE;
  }
  // x(2+3)
  if (self.type == BLOCK_START && last.type == LETTER) {
    return TRUE;
  }
  // All the functions
  if (is_function(self.type) &&
      (last.type == NUMBER || last.type == BLOCK_END || last.type == LETTER)) {
    return TRUE;
  }
  return FALSE;
}

static void remove_eo_idx(struct EquationObject *list, int length, int idx) {
  if (idx == length - 1) {
    list[idx].type = END_LEX;
  }

  for (int i = idx; i < length - 1; i++) {
    list[i] = list[i + 1];
  }
}

static Boolean is_negative(struct EquationObject self,
                           struct EquationObject last) {
  if (self.type == SUB) {
    if (last.type == NUMBER) {
      return FALSE;
    }
    if (last.type == BLOCK_END) {
      return FALSE;
    }
    if (last.type == LETTER) {
      return FALSE;
    }
  } else {
    return FALSE;
  }

  return TRUE;
}

double solve_const_expr(struct EquationObject *input, int length,
                        struct InputVar *buffer, int num_args) {
  // Get number of juxtaposed elements to know how long to make buffer
  int extra_count = 0;
  for (int i = 1; i < length; i++) {
    if (is_juxtaposed(input[i], input[i - 1])) {
      extra_count++;
    }
    if (is_negative(input[i], input[i - 1])) {
      extra_count++;
    }
  }

  // Make some tweaks such as negative numbers and juxtaposition into expression
  // as it is copied
  struct EquationObject expression[length + 2 * extra_count] = {};
  int new_len = length;
  int insert_idx = 0;
  int i = 0;

  // Negative sign first
  if (input[0].type == SUB) {
    expression[insert_idx].type = NUMBER;
    expression[insert_idx].value.number = -1.0;
    insert_idx++;
    expression[insert_idx].type = MULT;
    new_len++;
    insert_idx++;
    i++;
  }

  // Main loop
  while (i < new_len) {
    struct EquationObject obj = input[i];

    // Check for juxtaposition and negative numbers
    if (i != 0) {
      if (is_juxtaposed(input[i], input[i - 1])) {
        expression[insert_idx].type = MULT;
        insert_idx++;
        new_len++;
      }
      if (is_negative(input[i], input[i - 1])) {
        expression[insert_idx].type = NUMBER;
        expression[insert_idx].value.number = -1.0;
        insert_idx++;
        expression[insert_idx].type = MULT;
        new_len++;
        insert_idx++;
        i++;
        continue;
      }
    }

    if (obj.type == LETTER) {
      Boolean found = FALSE;
      for (int j = 0; j < num_args; j++) {
        struct Letter cur_letter = buffer[j].letter;
        if ((cur_letter.letter == obj.value.letter.letter) &&
            (cur_letter.subscript == obj.value.letter.subscript)) {
          expression[insert_idx].type = NUMBER;
          expression[insert_idx].value.number = buffer[j].value;
          found = TRUE;
        }
      }
      if (!found) {
        f_bad_equation = 1;
        return 0.0 / 0.0;
      }
    } else if (obj.type == PI_VAL) {
      expression[insert_idx].type = NUMBER;
      expression[insert_idx].value.number = PI;
    } else {
      expression[insert_idx] = input[i];
    }

    insert_idx++;
    i++;
  }

  // Debug
  for (int i = 0; i < new_len; i++) {
    print_eo(expression[i]);
  }

  // Use recursion to reduce blocks to doubles

  // Solve the rest in PEDMAS order

  // Functions
  for (int i = 0; i < new_len; i++) {
    Boolean exit = FALSE;
    double val = 0.0;
    if (i + 1 < new_len) {

      switch (expression[i].type) {
      case ROOT:
        val = nth_root(expression[i].value.number,
                       expression[i + 1].value.number);
        break;
      case EXP:
        break;
      case SINE:
        break;
      case COSINE:
        break;
      case TANGENT:
        break;
      case ARCSINE:
        break;
      case ARCCOSINE:
        break;
      case ARCTANGENT:
        break;
      case LOG:
        break;
      default:
        exit = TRUE;
        break;
      }
    }

    if (exit) {
      continue;
    }

    expression[i].type = NUMBER;
    expression[i].value.number = val;

    if (i + 1 >= new_len) {
      f_bad_equation = 1;
      return 0.0 / 0.0;
    }
    remove_eo_idx(expression, new_len, i + 1);
    new_len--;
  }

  return 0.0;
}
