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
  if (self.type == BLOCK_START && last.type == BLOCK_END) {
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
  // struct EquationObject out_buffer[buffer_len];

  // Get number of juxtaposed elements to know how long to make buffer
  int extra_count = 0;
  for (int i = 1; i < length; i++) {
    if (is_juxtaposed(input[i], input[i - 1])) {
      extra_count++;
    }
    if (is_negative(input[i], input[i - 1])) {
      extra_count += 3;
    }
    if ((input[i].type == NUMBER || input[i].type == LETTER ||
         input[i].type == BLOCK_START) &&
        input[i - 1].type == BLOCK_END) {
      int block_count = 1;
      int j = i - 2;
      int extra = 0;
      while (block_count != 0) {
        if (input[j].type == BLOCK_END) {
          block_count++;
        }
        if (input[j].type == BLOCK_START) {
          block_count--;
        }
        j--;
        extra++;
      }
      extra++;
      extra_count += extra * (buffer_len / extra);
    }
  }

  int new_len = length;
  int insert_idx = 0;
  int i = 0;

  Boolean seeking_close = FALSE;
  int negative_blocks = 0;

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
    if (insert_idx > buffer_len) {
      f_buffer_overflow = TRUE;
      return 0;
    }

    struct EquationObject obj = input[i];

    // Check for juxtaposition, negative numbers and add x^0 to constants
    if (i != 0) {
      if (is_juxtaposed(input[i], input[i - 1])) {
        out_buffer[insert_idx].type = MULT;
        insert_idx++;
        new_len++;
      }
      if (is_negative(input[i], input[i - 1])) {
        switch (input[i - 1].type) {
        case ROOT:
        case EXP:
        case SINE:
        case COSINE:
        case TANGENT:
        case ARCSINE:
        case ARCCOSINE:
        case ARCTANGENT:
        case LOG:
          out_buffer[insert_idx].type = BLOCK_START;
          new_len++;
          insert_idx++;
          seeking_close = TRUE;
          break;
        default:
          break;
        };

        out_buffer[insert_idx].type = NUMBER;
        out_buffer[insert_idx].value.number = -1.0;
        new_len++;
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
        return (int)nan;
      }
    } else if (obj.type == PI_VAL) {
      out_buffer[insert_idx].type = NUMBER;
      out_buffer[insert_idx].value.number = PI;
    } else {
      out_buffer[insert_idx] = input[i];
    }

    if (obj.type == BLOCK_START) {
      negative_blocks++;
    }
    if (obj.type == BLOCK_END) {
      negative_blocks--;
    }
    if (seeking_close) {
      switch (obj.type) {
      case NUMBER:
      case LETTER:
      case PI_VAL:
      case ROOT:
      case EXP:
      case BLOCK_START:
      // case BLOCK_END:
      case SINE:
      case COSINE:
      case TANGENT:
      case ARCSINE:
      case ARCCOSINE:
      case ARCTANGENT:
      case LOG:
        break;
      default:
        if (obj.type == SUB && is_negative(input[i], input[i - 1])) {
          break;
        }
        if (negative_blocks == 0) {
          out_buffer[insert_idx + 1] = out_buffer[insert_idx];
          out_buffer[insert_idx].type = BLOCK_END;
          new_len++;
          insert_idx++;
          seeking_close = FALSE;
        }
        break;
      }
    }

    insert_idx++;
    i++;
    if (obj.type == END_LEX) {
      break;
    }
  }

  return new_len;
}
