#include "../../include/solve_consts.h"
#include "../../include/atrig.h"
#include "../../include/dutils.h"
#include "../../include/enums.h"
#include "../../include/equation_objects.h"
#include "../../include/flags.h"
#include "../../include/log.h"
#include "../../include/parse.h"
#include "../../include/pow.h"
#include "../../include/root.h"
#include "../../include/trig.h"
#include "../../include/utils.h"

static Boolean is_function(enum EOType type) {
  switch (type) {
  case PI_VAL:
  case SINE:
  case COSINE:
  case TANGENT:
  case ARCSINE:
  case ARCCOSINE:
  case ARCTANGENT:
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
  // Almost all the functions
  if (is_function(self.type) && self.type != EXP &&
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

  // Use recursion to reduce blocks to doubles
  i = 0;
  Boolean blocks_found = FALSE;
  while (!blocks_found) {
    if (i >= new_len) {
      blocks_found = TRUE;
      break;
    }

    if (expression[i].type == BLOCK_END) {
      int start = i;
      while (start >= 0) {
        if (expression[start].type == BLOCK_START) {
          break;
        } else {
          start--;
        }
      }
      short count = i - start;
      struct EquationObject new_buffer[count];

      // Remove start block
      remove_eo_idx(expression, new_len, start);

      // New buffer and recursion
      for (int j = 0; j < count - 1; j++) {
        new_buffer[j] = expression[start];
        remove_eo_idx(expression, new_len, start);
        new_len--;
      }
      expression[start].type = NUMBER;
      expression[start].value.number =
          solve_const_expr(new_buffer, new_len + 1, buffer, num_args);
    }
    i++;
  }

  // Solve the rest in PEDMAS order

  // Functions
  // Going backwards because nested functions
  for (int i = new_len - 2; i >= 0; i--) {
    Boolean exit = FALSE;
    double val = 0.0;
    if (i + 1 < new_len) {
      switch (expression[i].type) {
      case SINE:
        val = sine(expression[i + 1].value.number);
        break;
      case COSINE:
        val = cosine(expression[i + 1].value.number);
        break;
      case TANGENT:
        val = tangent(expression[i + 1].value.number);
        break;
      case ARCSINE:
        val = arc_sine(expression[i + 1].value.number);
        break;
      case ARCCOSINE:
        val = arc_cosine(expression[i + 1].value.number);
        break;
      case ARCTANGENT:
        val = arc_tangent(expression[i + 1].value.number);
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

  // Exponentiation and friends
  i = 1;
  Boolean exp_found = FALSE;
  while (!exp_found) {
    if (i >= new_len) {
      exp_found = TRUE;
      break;
    }

    double before = expression[i - 1].value.number;
    double after = expression[i + 1].value.number;
    double val = 0.0;

    Boolean found = FALSE;

    if (expression[i].type == EXP) {
      if (dfloor(expression[i + 1].value.number) ==
          expression[i + 1].value.number) {
        val = pow_di(expression[i - 1].value.number,
                     (int)expression[i + 1].value.number);
      } else {
        val = pow_dd(expression[i - 1].value.number,
                     expression[i + 1].value.number);
      }
      found = TRUE;
    }

    if (expression[i].type == ROOT) {
      val = nth_root(expression[i - 1].value.number,
                     expression[i + 1].value.number);

      found = TRUE;
    }

    if (expression[i].type == LOG) {
      val =
          log_n(expression[i - 1].value.number, expression[i + 1].value.number);

      found = TRUE;
    }

    if (found) {
      expression[i].type = NUMBER;
      expression[i].value.number = val;
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      i = 1;
      continue;
    }
    i++;
  }

  // Multiplications and divisions
  i = 1;
  Boolean mult_found = FALSE;
  while (!mult_found) {
    if (i >= new_len) {
      mult_found = TRUE;
      break;
    }
    double before = expression[i - 1].value.number;
    double after = expression[i + 1].value.number;
    double val = 0.0;

    Boolean found = FALSE;

    if (expression[i].type == MULT) {
      val = before * after;
      found = TRUE;
    }
    if (expression[i].type == DIV) {
      val = before / after;
      found = TRUE;
    }

    if (found) {
      expression[i].type = NUMBER;
      expression[i].value.number = val;
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      i = 1;
      continue;
    }
    i++;
  }

  // Addition and subtraction
  i = 1;
  Boolean add_found = FALSE;
  while (!add_found) {
    if (i >= new_len) {
      add_found = TRUE;
      break;
    }

    double before = expression[i - 1].value.number;
    double after = expression[i + 1].value.number;
    double val = 0.0;

    Boolean found = FALSE;

    if (expression[i].type == ADD) {
      val = before + after;
      found = TRUE;
    }
    if (expression[i].type == SUB) {
      val = before - after;
      found = TRUE;
    }

    if (found) {
      expression[i].type = NUMBER;
      expression[i].value.number = val;
      remove_eo_idx(expression, new_len, i + 1);
      new_len--;
      remove_eo_idx(expression, new_len, i - 1);
      new_len--;
      i = 1;
      continue;
    }
    i++;
  }

  return expression[0].value.number;
}
