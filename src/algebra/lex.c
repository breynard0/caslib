#include "../../include/enums.h"
#include "../../include/objects.h"
#include "../../include/utils.h"

int lex(char *input, int length, struct EquationObject* buffer,
        int max_length) {
  int i = 0;
  int out_len = 0;

  short state = 1;

  struct EquationObject eo_def;
  struct EquationObject eo;
  enum Boolean dot_flag = FALSE;
  enum Boolean subscript = FALSE;

  short block_sp = 0;
  int blocks[64];

  while (i < length) {
    if (out_len + 1 >= max_length) {
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
          dot_flag = TRUE;
        } else {
          eo.value.number +=
              ((double)num) / double_digits_partial(eo.value.number);
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
        blocks[block_sp] = i;
        block_sp++;
        eo.type = BLOCK_START;
        blocks[block_sp] = i;
        state = 1;
      } else if (c == ')') {
        int start = blocks[block_sp];
        block_sp--;
        eo.type = BLOCK_END;
        eo.value.block.start = start;
        eo.value.block.count = i - start;
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
        if (subscript == TRUE) {
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
          dot_flag = TRUE;
        } else {
          eo.value.number +=
              ((double)num) / double_digits_partial(eo.value.number);
        }
        state = 2;
      } else if (c == '.') {
        dot_flag = TRUE;
        state = 2;
      } else {
        buffer[out_len] = eo;
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
      } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        eo.type = LETTER;
        if (subscript == TRUE) {
          eo.value.letter.letter = c;
        } else {
          eo.value.letter.subscript = c;
        }
        state = 3;
      } else {
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;

        i--;
        state = 1;
      }
      break;
    case 4:
      switch (c) {
      case 'p':
        eo.type = PI;
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
        state = 5;
        break;
      case 'l':
        eo.type = LOG;
        state = 6;
        break;
      }
      break;
    case 5:
      if (c >= '0' && c <= '9') {
        eo.value.number *= 10;
        eo.value.number += c - '0';
        state = 5;
      } else {
        eo.type = ROOT;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      }
      break;
    case 6:
      if (c >= '0' && c <= '9') {
        eo.value.number *= 10;
        eo.value.number += c - '0';
        state = 5;
      } else {
        eo.type = LOG;
        buffer[out_len] = eo;
        eo = eo_def;
        out_len++;
        state = 1;
      }
      break;
    }
    i++;
  }

  return out_len;
}
