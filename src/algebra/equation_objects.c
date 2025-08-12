#include "equation_objects.h"
#include "dutils.h"
#include "enums.h"
#include "expansion.h"
#include "parse.h"
#include "pow.h"
#include "utils.h"
#include "flags.h"

void remove_eo_idx(struct EquationObject *list, int length, int idx) {
  if (idx == length - 1) {
    list[idx].type = END_LEX;
  }

  for (int i = idx; i < length - 1; i++) {
    list[i] = list[i + 1];
  }
}

// Make sure list has enough space, pre-increment
void insert_eo_idx(struct EquationObject *list, int length, int idx,
                   struct EquationObject object) {
  for (int i = length - 2; i >= idx; i--) {
    list[i + 1] = list[i];
  }
  list[idx] = object;
}

// Yes, this is a wrapper. Yes, I'll fix it if it's a performance issue.
int div_terms(struct EquationObject *term0, int t0_len,
              struct EquationObject *term1, int t1_len) {
  struct EquationObject tmp_buf[2 * (t0_len + t1_len)];
  int tmp_len = 0;
  for (int i = 0; i < t0_len; i++) {
    tmp_buf[tmp_len] = term0[i];
    tmp_len++;
  }
  tmp_buf[tmp_len].type = DIV;
  tmp_len++;
  tmp_buf[tmp_len].type = BLOCK_START;
  tmp_len++;
  for (int i = 0; i < t1_len; i++) {
    tmp_buf[tmp_len] = term1[i];
    tmp_len++;
  }
  tmp_buf[tmp_len].type = BLOCK_END;
  tmp_len++;

  int new_len = expand_polynomial(tmp_buf, tmp_len);

  for (int i = 0; i < new_len; i++) {
    term0[i] = tmp_buf[i];
  }

  return new_len;
}

int get_polynomial_degree(struct EquationObject *expression, int length) {
  int highest_degree = 0;
  Boolean letter_found = FALSE;
  for (int i = 0; i < length; i++) {
    if (expression[i].type == EXP &&
        expression[i + 1].value.number > highest_degree) {
      highest_degree = expression[i + 1].value.number;
    } else if (expression[i].type == LETTER) {
      letter_found = TRUE;
    }
  }
  if (letter_found && highest_degree == 0) {
    highest_degree = 1;
  }

  return highest_degree;
}

int eo_to_string(struct EquationObject *expression, int length, char *buffer) {
  int out_len = 0;
  for (int i = 0; i < length; i++) {
    switch (expression[i].type) {
    case NUMBER: {
      double num = expression[i].value.number;
      // NaN check
      if (num != num) {
        f_undefined = TRUE;
        return 0;
      }
      double whole = (double)((long long)num);
      double partial = num - ((double)whole);
      long long digits = double_digits_whole(double_abs(whole));
      int last = 0;
      for (int i = 0; i < digits; i++) {
        long long digit = (long long)(whole / pow_di(10, digits - i - 1));
        digit -= last;
        if (digit < 0) {
          buffer[out_len] = '-';
          out_len++;
        }
        last *= 10;
        last += digit * 10;
        buffer[out_len] = long_long_abs(digit) + '0';
        out_len++;
      }
      if (double_abs(partial) > THRESHOLD) {
        buffer[out_len] = '.';
        out_len++;
        digits = double_digits_partial(num);
        for (int i = 0; i < digits; i++) {
          partial *= 10;
          buffer[out_len] = ((short)partial) + '0';
          out_len++;
        }
      }
      break;
    }
    case LETTER:
      buffer[out_len] = expression[i].value.letter.letter;
      out_len++;
      if (expression[i].value.letter.subscript != ' ') {
        buffer[out_len] = ' ';
        out_len++;
        buffer[out_len] = expression[i].value.letter.subscript;
        out_len++;
      }
      break;
    case MULT:
      if (!is_juxtaposed(expression[i + 1], expression[i - 1])) {
        buffer[out_len] = '*';
        out_len++;
      }
      break;
    case DIV:
      buffer[out_len] = '/';
      out_len++;
      break;
    case ADD:
      buffer[out_len] = '+';
      out_len++;
      break;
    case SUB:
      buffer[out_len] = '-';
      out_len++;
      break;
    case PI_VAL:
      buffer[out_len] = '@';
      out_len++;
      break;
    case ROOT:
      buffer[out_len] = '#';
      out_len++;
      break;
    case EXP:
      buffer[out_len] = '^';
      out_len++;
      break;
    case BLOCK_START:
      buffer[out_len] = '(';
      out_len++;
      break;
    case BLOCK_END:
      buffer[out_len] = ')';
      out_len++;
      break;
    case EQUAL:
      buffer[out_len] = '=';
      out_len++;
      break;
    case SINE:
      buffer[out_len] = '[';
      out_len++;
      break;
    case COSINE:
      buffer[out_len] = ']';
      out_len++;
      break;
    case TANGENT:
      buffer[out_len] = ';';
      out_len++;
      break;
    case ARCSINE:
      buffer[out_len] = '`';
      out_len++;
      break;
    case ARCCOSINE:
      buffer[out_len] = '~';
      out_len++;
      break;
    case ARCTANGENT:
      buffer[out_len] = '$';
      out_len++;
      break;
    case LOG:
      buffer[out_len] = '&';
      out_len++;
      break;
    case END_LEX:
      break;
    case NONE:
      break;
    }
  }
  return out_len;
}