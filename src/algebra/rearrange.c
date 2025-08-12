#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"
#include "gcf.h"

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

  struct EquationObject lhs[3 * length] = {};
  struct EquationObject rhs[3 * length] = {};
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
      if (i == lhs_len - 1) {
        start++;
        end++;
      }
      Boolean found = FALSE;
      while (start >= 0 && lhs[start].type != ADD && lhs[start].type != SUB) {
        if (lhs[start].type == LETTER &&
            lhs[start].value.letter.letter == target.letter &&
            lhs[start].value.letter.subscript == target.subscript) {
          found = TRUE;
        }
        start--;
      }
      start++;
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
        if (lhs[lhs_len - 1].type == ADD || lhs[lhs_len - 1].type == SUB) {
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
      if (i == rhs_len - 1) {
        start++;
      }
      Boolean found = FALSE;
      while (start >= 0 && rhs[start].type != ADD && rhs[start].type != SUB) {
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
  int max = lhs_len;
  if (rhs_len > lhs_len) {
    max = rhs_len;
  }
  for (int i = 0; i < max; i++) {
    for (int j = 0; j < r_objs_len; j++) {
      if (i < lhs_len && lhs[i].type == LETTER &&
          lhs[i].value.letter.letter == r_objs[j].letter.letter &&
          lhs[i].value.letter.subscript == r_objs[j].letter.subscript) {
        remove_eo_idx(lhs, lhs_len, i);
        lhs_len--;
        for (int k = 0; k <= r_objs[j].end_idx - r_objs[j].start_idx; k++) {
          lhs_len++;
          insert_eo_idx(lhs, lhs_len, i + k, original[r_objs[j].start_idx + k]);
        }
      }
      if (i < rhs_len && rhs[i].type == LETTER &&
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

  // Flatten -1 * num
  int i = 2;
  while (i < lhs_len) {
    if (lhs[i].type == NUMBER && lhs[i - 1].type == MULT &&
        lhs[i - 2].type == NUMBER) {
      lhs[i].value.number *= lhs[i - 2].value.number;
      remove_eo_idx(lhs, lhs_len, i - 2);
      lhs_len--;
      remove_eo_idx(lhs, lhs_len, i - 2);
      lhs_len--;
    }
    i++;
  }
  while (i < rhs_len) {
    if (rhs[i].type == NUMBER && rhs[i - 1].type == MULT &&
        rhs[i - 2].type == NUMBER) {
      rhs[i].value.number *= rhs[i - 2].value.number;
      remove_eo_idx(rhs, rhs_len, i - 2);
      rhs_len--;
      remove_eo_idx(rhs, rhs_len, i - 2);
      rhs_len--;
    }
    i++;
  }

  // // Divide by GCF
  struct EquationObject gcf[length] = {};
  int gcf_len = 0;
  int len0 = 0;
  int pos1 = 0;
  int len1 = 0;
  Boolean found = FALSE;
  Boolean calculated = FALSE;
  i = 0;
  while (i < lhs_len) {
    if (lhs[i].type == ADD || lhs[i].type == SUB || i >= lhs_len - 1) {
      if (found) {
        pos1++;
        len1++;
        gcf_len = term_gcf(lhs, len0, lhs + pos1, len1, gcf, length, target);
        len1 = 0;
        pos1 = i + 1;
        calculated = TRUE;
      } else {
        found = TRUE;
      }
    } else {
      if (found) {
        len1++;
      } else {
        pos1++;
        len0++;
      }
    }
    i++;
  }

  if (!calculated && found) {
    for (int j = 0; j < lhs_len; j++) {
      gcf[j] = lhs[j];
      gcf_len++;
    }
  }

  // Remove letter from GCF
  i = 0;
  while (i < gcf_len) {
    if (gcf[i].type == LETTER && gcf[i].value.letter.letter == target.letter &&
        gcf[i].value.letter.subscript == target.subscript) {
      remove_eo_idx(gcf, gcf_len, i);
      gcf_len--;
      if (i < gcf_len - 1 && gcf[i].type == EXP) {
        remove_eo_idx(gcf, gcf_len, i);
        gcf_len--;
        remove_eo_idx(gcf, gcf_len, i);
        gcf_len--;
      }
      if (i >= 0 && (gcf[i - 1].type == MULT || gcf[i - 1].type == DIV)) {
        remove_eo_idx(gcf, gcf_len, i - 1);
        gcf_len--;
      } else if (i < gcf_len && (gcf[i].type == MULT || gcf[i].type == DIV)) {
        remove_eo_idx(gcf, gcf_len, i);
        gcf_len--;
      }
    }
    i++;
  }
  
  // Do the division
  struct EquationObject bs_obj;
  bs_obj.type = BLOCK_START;

  lhs_len++;
  insert_eo_idx(lhs, lhs_len, 0, bs_obj);
  lhs[lhs_len].type = BLOCK_END;
  lhs_len++;
  lhs[lhs_len].type = DIV;
  lhs_len++;
  lhs[lhs_len].type = BLOCK_START;
  lhs_len++;

  rhs_len++;
  insert_eo_idx(rhs, rhs_len, 0, bs_obj);
  rhs[rhs_len].type = BLOCK_END;
  rhs_len++;
  rhs[rhs_len].type = DIV;
  rhs_len++;
  rhs[rhs_len].type = BLOCK_START;
  rhs_len++;

  i = 0;
  while (i < gcf_len) {
    lhs[lhs_len] = gcf[i];
    lhs_len++;
    rhs[rhs_len] = gcf[i];
    rhs_len++;

    i++;
  }

  lhs[lhs_len].type = BLOCK_END;
  lhs_len++;
  lhs[lhs_len].type = END_LEX;
  lhs_len++;
  lhs_len = expand_polynomial(lhs, lhs_len) - 1;

  rhs[rhs_len].type = BLOCK_END;
  rhs_len++;
  rhs[rhs_len].type = END_LEX;
  rhs_len++;
  rhs_len = expand_polynomial(rhs, rhs_len) - 1;

  // Take out term if all instances of target have the same degree
  // If they have different degrees, then too bad for you
  found = FALSE;
  Boolean correct = TRUE;
  i = 0;
  double degree = 0;
  while (i < lhs_len) {
    if (lhs[i].type == LETTER && lhs[i].value.letter.letter == target.letter &&
        lhs[i].value.letter.subscript == target.subscript) {
      double my_deg = 1;
      if (i < lhs_len - 1 && lhs[i + 1].type == EXP) {
        my_deg = lhs[i + 2].value.number;
      }
      if (found) {
        if (degree != my_deg) {
          correct = FALSE;
        }
      } else {
        found = TRUE;
      }
      degree = my_deg;
    }
    i++;
  }
  if (correct) {
    // Update rhs
    lhs_len++;
    insert_eo_idx(lhs, lhs_len, 0, bs_obj);
    lhs[lhs_len].type = BLOCK_END;
    lhs_len++;
    lhs[lhs_len].type = DIV;
    lhs_len++;
    lhs[lhs_len].type = LETTER;
    lhs[lhs_len].value.letter = target;
    lhs_len++;
    if (degree != 1) {
      lhs[lhs_len].type = EXP;
      lhs_len++;
      lhs[lhs_len].type = NUMBER;
      lhs[lhs_len].value.number = degree;
      lhs_len++;
    }
    lhs[lhs_len].type = END_LEX;
    lhs_len++;
    lhs_len = expand_polynomial(lhs, lhs_len) - 1;

    rhs_len++;
    insert_eo_idx(rhs, rhs_len, 0, bs_obj);
    rhs[rhs_len].type = BLOCK_END;
    rhs_len++;
    rhs[rhs_len].type = DIV;
    rhs_len++;
    rhs[rhs_len].type = BLOCK_START;
    rhs_len++;
    i = 0;
    Boolean found_add_sub = FALSE;
    while (i < lhs_len) {
      rhs[rhs_len] = lhs[i];
      rhs_len++;
      if (lhs[i].type == ADD || lhs[i].type == SUB) {
        found_add_sub = TRUE;
      }

      i++;
    }
    rhs[rhs_len].type = BLOCK_END;
    rhs_len++;
    if (!found_add_sub) {
      rhs[rhs_len].type = END_LEX;
      rhs_len++;
      rhs_len = expand_polynomial(rhs, rhs_len) - 1;
    }

    // Set lhs
    lhs_len = 0;

    lhs[lhs_len].type = LETTER;
    lhs[lhs_len].value.letter = target;
    lhs_len++;

    if (degree != 1) {
      lhs[lhs_len].type = EXP;
      lhs_len++;
      lhs[lhs_len].type = NUMBER;
      lhs[lhs_len].value.number = degree;
      lhs_len++;
    }
  }

  // Push
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
  // for (int i = 0; i < gcf_len; i++) {
  //   buffer[out_len] = gcf[i];
  //   out_len++;
  // }
  buffer[out_len].type = END_LEX;
  out_len++;

  return out_len;
}
