#include "buttons.h"
#include "draw.h"
#include "enums.h"
#include "equation_objects.h"
#include "letters.h"
#include "lex.h"
#include "parse.h"
#include "solve_consts.h"
#include "utils.h"

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
                           short buf_width, char *expression_in, int *expr_len,
                           struct SolveVar *values_buf, int *values_len,
                           Boolean *subscript, enum StateMode *mode,
                           short *cursor, enum PushButton button,
                           union PushButtonData data) {
  struct SolveVar cur_value = values_buf[*cursor];

  switch (button) {
  case B_NUMBER_LETTER:
    if (cur_value.num_len < 23) {
      cur_value.num[cur_value.num_len] = data.number + '0';
      cur_value.num_len++;
    }
    break;
  case B_PI:
    if (cur_value.num_len < 23) {
      cur_value.num[cur_value.num_len] = '@';
      cur_value.num_len++;
    }
    break;
  case B_DEL:
    if (cur_value.num_len > 0) {
      cur_value.num_len--;
    }
    break;
  case B_DOT:
    if (cur_value.num_len < 23) {
      cur_value.num[cur_value.num_len] = '.';
      cur_value.num_len++;
    }
    break;
  case B_UP:
    if (*cursor > 0) {
      (*cursor)--;
    }
    cur_value = values_buf[*cursor];
    break;
  case B_DOWN:
    if (*cursor < (*values_len - 1)) {
      (*cursor)++;
    }
    cur_value = values_buf[*cursor];
    break;
  case B_SOLVE:
    break;
  default:
    *mode = M_EXPRESSION;
    struct EquationObject expression[192] = {};
    int new_len = lex(expression_in, *expr_len, expression, 192);

    // Construct vars array
    struct InputVar vars[*values_len] = {};
    int vars_len = 0;
    for (int i = 0; i < *values_len; i++) {
      struct SolveVar val = values_buf[i];
      struct EquationObject num_obj[2] = {};
      lex(val.num, val.num_len, num_obj, 20);
      vars[vars_len].letter = val.letter;
      vars[vars_len].value = num_obj[0].value.number;
      vars_len++;
    }

    double solution = solve_const_expr(expression, new_len, vars, vars_len);
    struct EquationObject solution_eo = {};
    solution_eo.type = NUMBER;
    solution_eo.value.number = solution;
    *expr_len = eo_to_string(&solution_eo, 1, expression_in);
    *cursor = *expr_len;
    break;
  }

  draw_letter(cur_value.letter.letter, 8, y_spacer, size, buffer, buf_width);
  double subscript_buffer = 0.0;
  if (cur_value.letter.subscript != ' ') {
    draw_letter(cur_value.letter.subscript, 8 + size, y_spacer + (size - 8),
                size - 8, buffer, buf_width);
    subscript_buffer = size - 8;
  }
  draw_letter('=', 8 + size + subscript_buffer, y_spacer, size, buffer,
              buf_width);

  short big = 32000;
  draw_expression(8 + 2 * size + subscript_buffer, y_spacer, size, buffer,
                  buf_width, cur_value.num, cur_value.num_len, &big, FALSE,
                  FALSE);

  values_buf[*cursor] = cur_value;
}

void update_show_roots(short y_spacer, short size, char *buffer,
                       short buf_width, double *roots, int roots_len,
                       enum StateMode *mode, short *cursor,
                       enum PushButton button, struct Letter letter,
                       short height) {
  switch (button) {
  case B_UP:
    if (*cursor > 0) {
      (*cursor)--;
    }
    break;
  case B_DOWN:
    if (*cursor < roots_len - 2) {
      (*cursor)++;
    }
    break;
  case B_GET_ROOT:
    break;
  default:
    *mode = M_EXPRESSION;
    *cursor = 0;
    break;
  }

  short offset = *cursor * size;
  if (roots_len == 0) {
    draw_letter('n', 8, y_spacer, size, buffer, buf_width);
    draw_letter('o', 8 + size, y_spacer, size, buffer, buf_width);
    draw_letter('r', 8 + 3*size, y_spacer, size, buffer, buf_width);
    draw_letter('o', 8 + 4*size, y_spacer, size, buffer, buf_width);
    draw_letter('o', 8 + 5*size, y_spacer, size, buffer, buf_width);
    draw_letter('t', 8 + 6*size, y_spacer, size, buffer, buf_width);
    draw_letter('s', 8 + 7*size, y_spacer, size, buffer, buf_width);
  }
  for (int i = 0; i < roots_len; i++) {
    short y = y_spacer - offset + (i * (size + 2));
    if (y + size < height) {
      draw_letter(letter.letter, 8, y, size, buffer, buf_width);
      // Set to 1 by default to give a little extra space between subscript and
      // index
      short subset_offset = 1;
      if (letter.subscript != ' ') {
        draw_letter(letter.subscript, 8 + size, y + (size - 8), size - 8,
                    buffer, buf_width);
        subset_offset += size - 8;
      }
      draw_letter(i + '0', 8 + size + subset_offset + 1, y + (size - 8),
                  size - 8, buffer, buf_width);
      draw_letter('=', 8 + 2 * size + subset_offset, y, size, buffer,
                  buf_width);
      struct EquationObject root_eo = {};
      root_eo.type = NUMBER;
      root_eo.value.number = roots[i];
      char str_buf[24] = "";
      int str_len = eo_to_string(&root_eo, 1, str_buf);
      draw_expression(8 + 3 * size + subset_offset, y, size, buffer, buf_width,
                      str_buf, str_len, cursor, FALSE, FALSE);
    }
  }
}