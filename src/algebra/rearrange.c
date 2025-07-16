#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"

struct ReplaceObject {
  struct Letter letter;
  short start_idx;
  short end_idx;
};

int rearrange_for_var(struct EquationObject *buffer, int length,
                      struct Letter target) {
  // Grab copy of original
  struct EquationObject original[length] = {};
  for (int i = 0; i < length; i++) {
    original[i] = buffer[i];
  }
  
  // Construct ReplaceObject array
  struct ReplaceObject r_objs[length / 2] = {};
  int new_len = length;
  int r_objs_len = 0;
  for (int i = 0; i < length; i++) {
    struct ReplaceObject temp;
    temp.letter.letter = '#';
    temp.letter.subscript = r_objs_len;

    switch (buffer[i].type) {
    // Single character values
    case PI_VAL:
    case NUMBER:
      if (i == 0 ||
          !(buffer[i - 1].type == SINE || buffer[i - 1].type == COSINE ||
            buffer[i - 1].type == TANGENT || buffer[i - 1].type == ARCSINE ||
            buffer[i - 1].type == ARCCOSINE ||
            buffer[i - 1].type == ARCTANGENT || buffer[i - 1].type == TANGENT ||
            buffer[i - 1].type == ARCSINE || buffer[i + 1].type == ROOT ||
            buffer[i - 1].type == ROOT || buffer[i + 1].type == EXP ||
            buffer[i - 1].type == EXP || buffer[i + 1].type == LOG ||
            buffer[i - 1].type == LOG)) {
        temp.start_idx = i;
        temp.end_idx = i;
        buffer[i].type = LETTER;
        buffer[i].value.letter = temp.letter;
        r_objs[r_objs_len] = temp;
        r_objs_len++;
      }
      break;

    // Blocks
    // Functions assume that these will be taken care of
    case BLOCK_END: {
      int start = i;
      while (buffer[start].type != BLOCK_START) {
        start--;
      }
      temp.start_idx = start;
      temp.end_idx = i;
      buffer[i].type = LETTER;
      buffer[i].value.letter = temp.letter;
      for (int j = 0; j < i - start; j++) {
        remove_eo_idx(buffer, new_len, start);
        new_len--;
      }
      r_objs[r_objs_len] = temp;
      r_objs_len++;
    } break;

    // Single input functions
    case SINE:
    case COSINE:
    case TANGENT:
    case ARCSINE:
    case ARCCOSINE:
    case ARCTANGENT:
      temp.start_idx = i;
      temp.end_idx = i + 1;
      buffer[i].type = LETTER;
      buffer[i].value.letter = temp.letter;
      remove_eo_idx(buffer, new_len, i + 1);
      new_len--;
      r_objs[r_objs_len] = temp;
      r_objs_len++;
      break;
    // Double input functions
    case ROOT:
    case LOG:
    case EXP:
      if (buffer[i - 1].type != LETTER && buffer[i + 1].type != LETTER) {
        temp.start_idx = i - 1;
        temp.end_idx = i + 1;
        buffer[i - 1].type = LETTER;
        buffer[i].value.letter = temp.letter;
        remove_eo_idx(buffer, new_len, i);
        new_len--;
        remove_eo_idx(buffer, new_len, i);
        new_len--;
        r_objs[r_objs_len] = temp;
        r_objs_len++;
      }
      break;
    default:
      break;
    }
  }

  int eq_pos = 0;
  while (buffer[eq_pos].type != EQUAL && eq_pos < new_len) {
    eq_pos++;
  }

  struct EquationObject lhs[2 * length] = {};
  struct EquationObject rhs[2 * length] = {};
  int lhs_len = 0;
  int rhs_len = 0;

  for (int i = 0; i < new_len; i++) {
    if (i < eq_pos) {
      lhs[lhs_len] = buffer[i];
      lhs_len++;
    }
    if (i > eq_pos) {
      rhs[rhs_len] = buffer[i];
      rhs_len++;
    }
  }
  lhs[lhs_len].type = END_LEX;
  lhs_len++;
  rhs[rhs_len].type = END_LEX;
  rhs_len++;

  lhs_len = expand_polynomial(lhs, lhs_len) - 1;
  rhs_len = expand_polynomial(rhs, rhs_len) - 1;

  for (int i = 0; i < lhs_len; i++) {
    if (lhs[i].type == ADD || lhs[i].type == SUB || i == lhs_len - 1) {
      int end = i;
      int start = end - 1;
      Boolean found = FALSE;
      while (start > 0 && lhs[start].type != ADD && lhs[start].type != SUB) {
        if (lhs[start].type == LETTER &&
            lhs[start].value.letter.letter == target.letter &&
            lhs[start].value.letter.subscript == target.subscript) {
          found = TRUE;
        }
        start--;
      }
      if (!found) {
        enum EOType sign = SUB;
        if (start != -1 &&
            ((lhs[start].type == NUMBER && lhs[start].value.number > 0) ||
             (lhs[start].type == SUB))) {
          sign = ADD;
        }
        if (start < 0) {
          start = 0;
        }

        if (rhs_len != 0 || sign == SUB) {
          rhs[rhs_len].type = sign;
          rhs_len++;
        }

        for (int j = 0; j < end - start; j++) {
          if (lhs[start].type == ADD || lhs[start].type == SUB) {
            j++;
          }
          rhs[rhs_len] = lhs[start];
          rhs_len++;
          remove_eo_idx(lhs, lhs_len, start);
          lhs_len--;
        }
        if (end == 0) {
          rhs[rhs_len] = lhs[0];
          rhs_len++;
          remove_eo_idx(lhs, lhs_len, 0);
          lhs_len--;
        }
      }
    }
  }

  for (int i = 0; i < rhs_len; i++) {
    if (rhs[i].type == ADD || rhs[i].type == SUB || i == rhs_len - 1) {
      int end = i;
      int start = end - 1;
      Boolean found = FALSE;
      while (start > 0 && rhs[start].type != ADD && rhs[start].type != SUB) {
        if (rhs[start].type == LETTER &&
            rhs[start].value.letter.letter == target.letter &&
            rhs[start].value.letter.subscript == target.subscript) {
          found = TRUE;
        }
        start--;
      }
      if (found) {
        enum EOType sign = SUB;
        if (start != -1 &&
            ((rhs[start].type == NUMBER && rhs[start].value.number > 0) ||
             (rhs[start].type == SUB))) {
          sign = ADD;
        }
        if (start < 0) {
          start = 0;
        }

        if (rhs_len != 0 || sign == SUB) {
          lhs[lhs_len].type = sign;
          lhs_len++;
        }

        for (int j = 0; j < end - start; j++) {
          if (rhs[start].type == ADD || rhs[start].type == SUB) {
            j++;
          }
          lhs[lhs_len] = rhs[start];
          lhs_len++;
          remove_eo_idx(rhs, rhs_len, start);
          rhs_len--;
        }
        if (end == 0) {
          lhs[lhs_len] = rhs[0];
          lhs_len++;
          remove_eo_idx(rhs, rhs_len, 0);
          rhs_len--;
        }
      }
    }
  }

  // Fix arrays
  struct EquationObject mult_obj;
  mult_obj.type = MULT;
  struct EquationObject num_obj;
  num_obj.type = NUMBER;
  num_obj.value.number = -1;

  // Left-hand side
  {
    struct EquationObject self = lhs[1];
    struct EquationObject last = lhs[0];

    if (self.type == NUMBER) {
      if (last.type == SUB) {
        lhs[1].value.number *= -1;
        remove_eo_idx(lhs, lhs_len, 0);
        lhs_len--;
      }
      if (last.type == ADD) {
        remove_eo_idx(lhs, lhs_len, 0);
        lhs_len--;
      }
    }
    if (self.type == LETTER) {
      if (last.type == SUB) {
        lhs_len++;
        insert_eo_idx(lhs, lhs_len, 1, num_obj);
        lhs_len++;
        insert_eo_idx(lhs, lhs_len, 1, mult_obj);
        remove_eo_idx(lhs, lhs_len, 0);
        lhs_len--;
      }
      if (last.type == ADD) {
        remove_eo_idx(lhs, lhs_len, 0);
        lhs_len--;
      }
    }
  }

  // Right-hand side
  {
    struct EquationObject self = rhs[1];
    struct EquationObject last = rhs[0];

    if (self.type == NUMBER) {
      if (last.type == SUB) {
        rhs[1].value.number *= -1;
        remove_eo_idx(rhs, rhs_len, 0);
        rhs_len--;
      }
      if (last.type == ADD) {
        remove_eo_idx(rhs, rhs_len, 0);
        rhs_len--;
      }
    }
    if (self.type == LETTER) {
      if (last.type == SUB) {
        rhs_len++;
        insert_eo_idx(rhs, rhs_len, 1, num_obj);
        rhs_len++;
        insert_eo_idx(rhs, rhs_len, 1, mult_obj);
        remove_eo_idx(rhs, rhs_len, 0);
        rhs_len--;
      }
      if (last.type == ADD) {
        remove_eo_idx(rhs, rhs_len, 0);
        rhs_len--;
      }
    }
  }
  
  // Expand both sides
  lhs[lhs_len].type = END_LEX;
  lhs_len++;
  lhs_len = expand_polynomial(lhs, lhs_len) - 1;
  rhs[rhs_len].type = END_LEX;
  rhs_len++;
  rhs_len = expand_polynomial(rhs, rhs_len) - 1;

  // Substitute in values
  for (int i = 0; i < lhs_len; i++) {
    for (int j = 0; j < r_objs_len; j++) {
      if (lhs[i].type == LETTER &&
          lhs[i].value.letter.letter == r_objs[j].letter.letter &&
          lhs[i].value.letter.subscript == r_objs[j].letter.subscript) {
        remove_eo_idx(lhs, lhs_len, i);
        lhs_len--;
        for (int k = 0; k <= r_objs[j].end_idx - r_objs[j].start_idx; k++) {
          lhs_len++;
          insert_eo_idx(lhs, lhs_len, i + k, original[r_objs[j].start_idx + k]);
        }
      }
      if (rhs[i].type == LETTER &&
          rhs[i].value.letter.letter == r_objs[j].letter.letter &&
          rhs[i].value.letter.subscript == r_objs[j].letter.subscript) {
        remove_eo_idx(rhs, rhs_len, i);
        rhs_len--;
        for (int k = 0; k <= r_objs[j].end_idx - r_objs[j].start_idx; k++) {
          rhs_len++;
          insert_eo_idx(rhs, rhs_len, i + k, original[r_objs[j].start_idx + k]);
        }
      }
    }
  }

  int out_len = 0;
  for (int i = 0; i < lhs_len; i++) {
    buffer[out_len] = lhs[i];
    out_len++;
  }
  buffer[out_len].type = EQUAL;
  out_len++;
  for (int i = 0; i < rhs_len; i++) {
    buffer[out_len] = rhs[i];
    out_len++;
  }
  buffer[out_len].type = END_LEX;
  out_len++;

  return out_len;
}
