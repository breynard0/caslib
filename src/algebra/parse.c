#include "parse.h"
#include "enums.h"
#include "equation_objects.h"
#include "flags.h"
#include "utils.h"

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

Boolean is_juxtaposed(struct EquationObject self, struct EquationObject last) {
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
  // xy
  if (self.type == LETTER && last.type == LETTER) {
    return TRUE;
  }
  // Almost all the functions
  if (is_function(self.type) && self.type != EXP &&
      (last.type == NUMBER || last.type == BLOCK_END || last.type == LETTER)) {
    return TRUE;
  }
  return FALSE;
}

Boolean is_negative(struct EquationObject self, struct EquationObject last) {
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

int expand_juxtopposed(struct EquationObject *input, int length,
                       struct EquationObject *out_buffer, int buffer_len,
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

  int new_len = length;
  int insert_idx = 0;
  int i = 0;

  // Negative sign first
  if (input[0].type == SUB) {
    out_buffer[insert_idx].type = NUMBER;
    out_buffer[insert_idx].value.number = -1.0;
    insert_idx++;
    out_buffer[insert_idx].type = MULT;
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
        out_buffer[insert_idx].type = MULT;
        insert_idx++;
        new_len++;
      }
      if (is_negative(input[i], input[i - 1])) {
        out_buffer[insert_idx].type = NUMBER;
        out_buffer[insert_idx].value.number = -1.0;
        insert_idx++;
        out_buffer[insert_idx].type = MULT;
        new_len++;
        insert_idx++;
        i++;
        continue;
      }
    }

    if (obj.type == LETTER && num_args > 0) {
      Boolean found = FALSE;
      for (int j = 0; j < num_args; j++) {
        struct Letter cur_letter = buffer[j].letter;
        if ((cur_letter.letter == obj.value.letter.letter) &&
            (cur_letter.subscript == obj.value.letter.subscript)) {
          out_buffer[insert_idx].type = NUMBER;
          out_buffer[insert_idx].value.number = buffer[j].value;
          found = TRUE;
        }
      }
      if (!found) {
        f_bad_equation = 1;
        double nan = NAN;
        return (int) nan;
      }
    } else if (obj.type == PI_VAL) {
      out_buffer[insert_idx].type = NUMBER;
      out_buffer[insert_idx].value.number = PI;
    } else {
      out_buffer[insert_idx] = input[i];
    }

    insert_idx++;
    i++;
  }

  return new_len;
}
