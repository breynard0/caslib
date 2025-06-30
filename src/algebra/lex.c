#include "../../include/debug.h"
#include "../../include/dutils.h"
#include "../../include/enums.h"
#include "../../include/equation_objects.h"
#include "../../include/pow.h"
#include "../../include/utils.h"

int lex(char *input, int length, struct EquationObject *buffer,
        int max_length) {
  int i = 0;
  int out_len = 0;

  short state = 1;

  struct EquationObject eo_def;
  eo_def.value.none = 0;
  struct EquationObject eo;

  Boolean dot_flag = FALSE;
  short decimal_digits = 0;

  Boolean subscript = FALSE;

  while (i <= length) {
    if (out_len + 3 >= max_length) {
      break;
    }

    char c = input[i];

    switch (state) {
    case 1:
      if (c >= '0' && c <= '9') {
        short num = c - '0';
        eo.type = NUMBER;
        if (dot_flag == FALSE) {
          eo.value.number *= 10;
          eo.value.number += num;
        } else {
          eo.value.number +=
              ((double)num) /
              pow_di(10.0, -double_digits_partial(eo.value.number));
          decimal_digits++;
        }
        state = 2;
      } else if (c == '.') {
        dot_flag = TRUE;
        state = 2;
      } else if (c == ':') {
        subscript = TRUE;
        state = 3;
      } else if (c == '*') {
        eo.type = MULT;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      } else if (c == '/') {
        eo.type = DIV;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      } else if (c == '+') {
        eo.type = ADD;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      } else if (c == '-') {
        eo.type = SUB;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      } else if (c == '^') {
        eo.type = EXP;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      } else if (c == '\\') {
        state = 4;
      } else if (c == '(') {
        eo.type = BLOCK_START;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      } else if (c == ')') {
        eo.type = BLOCK_END;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      } else if (c == '=') {
        eo.type = EQUAL;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      } else {
        eo.type = LETTER;

        // Default values
        eo.value.letter.letter = ' ';
        eo.value.letter.subscript = ' ';

        if (subscript == FALSE) {
          eo.value.letter.letter = c;
        } else {
          eo.value.letter.subscript = c;
        }
        state = 3;
      }
      break;
    case 2:
      if (c >= '0' && c <= '9') {
        short num = c - '0';
        eo.type = NUMBER;
        if (dot_flag == FALSE) {
          eo.value.number *= 10;
          eo.value.number += num;
        } else {
          eo.value.number =
              dfloor(eo.value.number * pow_di(10.0, decimal_digits)) /
              pow_di(10.0, decimal_digits);
          eo.value.number += ((double)num) / pow_di(10.0, decimal_digits + 1);
          decimal_digits++;
        }
        state = 2;
      } else if (c == '.') {
        dot_flag = TRUE;
        state = 2;
      } else {
        buffer[out_len] = eo;
        dot_flag = FALSE;
        decimal_digits = 0;
        eo = eo_def;
        out_len++;

        i--;
        state = 1;
      }
      break;
    case 3:
      if (c == ':') {
        subscript = TRUE;
        state = 3;
      } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                 subscript == TRUE) {
        eo.type = LETTER;
        if (subscript == FALSE) {
          eo.value.letter.letter = c;
        } else {
          eo.value.letter.subscript = c;
          subscript = FALSE;
        }
        state = 3;
      } else {
        buffer[out_len] = eo;
        subscript = FALSE;
        eo = eo_def;
        out_len++;

        i--;
        state = 1;
      }
      break;
    case 4:
      switch (c) {
      case 'p':
        eo.type = PI_VAL;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
        break;
      case 's':
        eo.type = SINE;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
        break;
      case 'c':
        eo.type = COSINE;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
        break;
      case 't':
        eo.type = TANGENT;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
        break;
      case 'a':
        eo.type = ARCSINE;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
        break;
      case 'o':
        eo.type = ARCCOSINE;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
        break;
      case 'g':
        eo.type = ARCTANGENT;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
        break;
      case 'r':
        eo.type = ROOT;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
        break;
      case 'l':
        eo.type = LOG;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
        break;
      }
      break;
    }
    i++;
  }

  buffer[out_len] = eo;
  eo = eo_def;
  eo.type = END_LEX;
  buffer[out_len] = eo;

  return out_len;
}
