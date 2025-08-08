#include "buttons.h"
#include "enums.h"

short button_update(char *buffer, int *length, short cursor_pos,
                    enum PushButton button, union PushButtonData data,
                    Boolean second, Boolean subscript) {
  short cursor = cursor_pos;
  switch (button) {
  case B_SIN:
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 's';
    (*length)++;
    cursor++;
    break;
  case B_COS:
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 'c';
    (*length)++;
    cursor++;
    break;
  case B_TAN:
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 't';
    (*length)++;
    cursor++;
    break;
  case B_ASIN:
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 'a';
    (*length)++;
    cursor++;
    break;
  case B_ACOS:
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 'o';
    (*length)++;
    cursor++;
    break;
  case B_ATAN:
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 'g';
    (*length)++;
    cursor++;
    break;
  case B_SQRT:
    buffer[cursor] = '2';
    (*length)++;
    cursor++;
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 'r';
    (*length)++;
    cursor++;
    break;
  case B_CBRT:
    buffer[cursor] = '3';
    (*length)++;
    cursor++;
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 'r';
    (*length)++;
    cursor++;
    break;
  case B_ROOT:
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 'r';
    (*length)++;
    cursor++;
    break;
  case B_SQR:
    buffer[cursor] = '^';
    (*length)++;
    cursor++;
    buffer[cursor] = '2';
    (*length)++;
    cursor++;
    break;
  case B_CB:
    buffer[cursor] = '^';
    (*length)++;
    cursor++;
    buffer[cursor] = '3';
    (*length)++;
    cursor++;
    break;
  case B_POW:
    buffer[cursor] = '^';
    (*length)++;
    cursor++;
    break;
  case B_LOG:
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 'l';
    (*length)++;
    cursor++;
    break;
  case B_NUMBER_LETTER:
    if (subscript) {
      buffer[cursor] = ':';
      (*length)++;
      cursor++;
    }

    if (second) {
      char letter = 'a';
      switch (data.number) {
      case 7:
        letter = 'a';
        break;
      case 8:
        letter = 'b';
        break;
      case 9:
        letter = 'c';
        break;
      case 4:
        letter = 'd';
        break;
      case 5:
        letter = 'e';
        break;
      case 6:
        letter = 'f';
        break;
      case 1:
        letter = 'x';
        break;
      case 2:
        letter = 'y';
        break;
      case 3:
        letter = 'w';
        break;
      }
      buffer[cursor] = letter;
    } else {
      buffer[cursor] = data.number + '0';
    }
    (*length)++;
    cursor++;
    break;
  case B_PLUS:
    buffer[cursor] = '+';
    (*length)++;
    cursor++;
    break;
  case B_MINUS:
    buffer[cursor] = '-';
    (*length)++;
    cursor++;
    break;
  case B_MULT:
    buffer[cursor] = '*';
    (*length)++;
    cursor++;
    break;
  case B_DIV:
    buffer[cursor] = '/';
    (*length)++;
    cursor++;
    break;
  case B_BLOCK_START:
    buffer[cursor] = '(';
    (*length)++;
    cursor++;
    break;
  case B_BLOCK_END:
    buffer[cursor] = ')';
    (*length)++;
    cursor++;
    break;
  case B_EQUAL:
    buffer[cursor] = '=';
    (*length)++;
    cursor++;
    break;
  case B_NEGATE:
    break;
  case B_SOLVE:
    break;
  case B_REARRANGE:
    break;
  case B_GET_ROOT:
    break;
  case B_EXPAND:
    break;
  case B_SUBSCRIPT:
    break;
  case B_CAPITAL:
    break;
  case B_2ND:
    break;
  case B_PI:
    buffer[cursor] = '\\';
    (*length)++;
    cursor++;
    buffer[cursor] = 'p';
    (*length)++;
    cursor++;
    break;
  case B_UP:
    break;
  case B_DOWN:
    break;
  case B_LEFT:
    cursor--;
    break;
  case B_RIGHT:
    cursor++;
    break;
  case B_DEL:
    if (cursor != *length) {
      for (int i = cursor; i < *length - 1; i++) {
        buffer[i] = buffer[i + 1];
      }
    }

    if (length >= 0) {
      (*length)--;
    }
    break;
  case B_CLEAR:
    *length = 0;
    cursor = 0;
    buffer[0] = '\0';
    break;
  case B_DEG:
    buffer[*length] = '\\';
    (*length)++;
    cursor++;
    buffer[*length] = 'd';
    (*length)++;
    cursor++;
    break;
  case B_DOT:
    buffer[*length] = '.';
    (*length)++;
    cursor++;
    break;
  case B_NONE:
    break;
  }

  return cursor;
}
