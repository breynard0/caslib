#include "buttons.h"
#include "draw.h"
#include "enums.h"
#include "equation_objects.h"
#include "letters.h"

Boolean update_get_var(short y_spacer, short size, char *buffer,
                       short buf_width, struct Letter *letter_buf,
                       Boolean *subscript, Boolean *second,
                       enum StateMode *mode, short *cursor,
                       enum PushButton button, union PushButtonData data) {
  struct Letter letter = *letter_buf;
  short x = buf_width / 8;
  switch (button) {
  case B_NUMBER_LETTER: {
    char push_letter = ' ';
    if (*second) {
      switch (data.number) {
      case 7:
        push_letter = 'a';
        break;
      case 8:
        push_letter = 'b';
        break;
      case 9:
        push_letter = 'c';
        break;
      case 4:
        push_letter = 'd';
        break;
      case 5:
        push_letter = 'e';
        break;
      case 6:
        push_letter = 'f';
        break;
      case 1:
        push_letter = 'x';
        break;
      case 2:
        push_letter = 'y';
        break;
      case 3:
        push_letter = 'a';
        break;
      }
    } else {
      push_letter = data.number + '0';
    }
    if (letter.letter == ' ') {
      letter.letter = push_letter;
    } else if (letter.subscript == ' ') {
      letter.subscript = push_letter;
    }
    break;
  }
  case B_DEL:
    if (letter.subscript != ' ') {
      letter.subscript = ' ';
    } else if (letter.letter != ' ') {
      letter.letter = ' ';
    }
  case B_2ND:
  case B_SUBSCRIPT:
  case B_NONE:
  case B_REARRANGE:
    break;
  default:
    *mode = M_EXPRESSION;
    return TRUE;
    break;
  }

  if (letter.letter == ' ') {
    draw_line(x, y_spacer + size, x + size, y_spacer + size, buffer, buf_width);
  } else {
    draw_letter(letter.letter, x, y_spacer, size, buffer, buf_width);
    if (letter.subscript == ' ') {
      draw_line(x + size, y_spacer + size, x + size + (size - 8),
                y_spacer + size, buffer, buf_width);
    } else {
      draw_letter(letter.subscript, x + size, y_spacer + (size - 8), size - 8,
                  buffer, buf_width);
    }
  }

  letter_buf->letter = letter.letter;
  letter_buf->subscript = letter.subscript;

  return FALSE;
}

void update_get_var_values(short y_spacer, short size, char *buffer,
                           short buf_width, char *expression_in, short expr_len,
                           double *values_buf, int *values_len,
                           Boolean *subscript, enum StateMode *mode,
                           short *cursor, enum PushButton button);
void update_show_roots(short y_spacer, short size, char *buffer,
                       short buf_width, double *roots, int roots_len,
                       Boolean *subscript, enum StateMode *mode, short *cursor,
                       enum PushButton button);