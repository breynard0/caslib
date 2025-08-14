#include "alt_buttons.h"
#include "buttons.h"
#include "draw.h"
#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"
#include "flags.h"
#include "letters.h"
#include "lex.h"
#include "parse.h"
#include "rearrange.h"
#include "root.h"
#include "valid.h"

#include <raylib.h>

#define WIDTH 192
#define HEIGHT 64
#define SIZE WIDTH *HEIGHT

Boolean draw_button(char *text, int x, int y, int width, int height,
                    int mouse_x, int mouse_y, Boolean pressed, Boolean second,
                    char *alt_text) {
  Boolean hover = FALSE;
  if (mouse_x >= x && mouse_x <= x + width && mouse_y >= y &&
      mouse_y <= y + height) {
    hover = TRUE;
  }

  Color fill_col = DARKGRAY;
  if (hover) {
    fill_col = DARKBLUE;
    if (pressed) {
      fill_col = BLUE;
    }
  }

  DrawRectangle(x, y, width, height, fill_col);
  DrawRectangleLines(x, y, width, height, LIGHTGRAY);

  int text_height = height / 3;
  if (width < height) {
    text_height = width / 3;
  }

  int text_width = MeasureText(text, text_height);

  while (text_width > width) {
    text_width = MeasureText(text, text_height);
    text_height -= 4;
  }

  if (second && !(alt_text[0] == 'n' && alt_text[1] == 'u' &&
                  alt_text[2] == 'l' && alt_text[3] == 'l')) {
    text = alt_text;
  }

  DrawText(text, x + (width - text_width) / 2.0, y + (height - text_height) / 2,
           text_height, WHITE);

  return hover;
}

void debug_display() {
  // I'll design the drawing code able to be turned into a bitboard if memory
  // usage requires it. Right now though, using chars would use ~12kb out of
  // 128kb, so I won't optimize until I have to
  char buffer[SIZE] = {};

  clear_display(buffer, SIZE);

  // for (int i = 0; i < HEIGHT; i++) {
  //   set_pixel(i, i, ON, buffer, WIDTH);
  // }
  // draw_line(0, 20, 100, 20, buffer, WIDTH);
  // draw_line(20, 0, 20, HEIGHT / 2, buffer, WIDTH);
  // draw_line(0, 0, 128, 4, buffer, WIDTH);
  // draw_line(0, 0, 128, 40, buffer, WIDTH);
  // draw_line(128, 4, 128, 40, buffer, WIDTH);
  // draw_line(11, 11, 11, 10+40-1, buffer, WIDTH);
  // draw_line(20, 20, 10, 32, buffer, WIDTH);
  // draw_line(10, 20, 80, 20, buffer, WIDTH);
  // draw_line(10, 20, 80, 20, buffer, WIDTH);
  // draw_line(20, 20, 10, 10, buffer, WIDTH);
  // draw_rect(10, 5, 172, 54, buffer, WIDTH);

  // draw_letter('0', 0, 6, 16, buffer, WIDTH);
  // draw_letter('1', 16, 6, 16, buffer, WIDTH);
  // draw_letter('2', 32, 6, 16, buffer, WIDTH);
  // draw_letter('3', 48, 6, 16, buffer, WIDTH);
  // draw_letter('4', 64, 6, 16, buffer, WIDTH);
  // draw_letter('5', 80, 6, 16, buffer, WIDTH);
  // draw_letter('6', 96, 6, 16, buffer, WIDTH);
  // draw_letter('7', 112, 6, 16, buffer, WIDTH);
  // draw_letter('8', 128, 6, 16, buffer, WIDTH);
  // draw_letter('9', 144, 6, 16, buffer, WIDTH);
  // draw_letter('@', 160, 6, 16, buffer, WIDTH);
  // draw_letter('%', 176, 6, 16, buffer, WIDTH);

  // draw_letter('#', 0, 28, 16, buffer, WIDTH);
  // draw_letter('a', 16, 28, 16, buffer, WIDTH);
  // draw_letter('b', 32, 28, 16, buffer, WIDTH);
  // draw_letter('c', 48, 28, 16, buffer, WIDTH);
  // draw_letter('d', 64, 28, 16, buffer, WIDTH);
  // draw_letter('e', 80, 28, 16, buffer, WIDTH);
  // draw_letter('f', 96, 28, 16, buffer, WIDTH);
  // draw_letter('g', 112, 28, 16, buffer, WIDTH);
  // draw_letter('i', 128, 28, 16, buffer, WIDTH);
  // draw_letter('l', 144, 28, 16, buffer, WIDTH);
  // draw_letter('n', 160, 28, 16, buffer, WIDTH);
  // draw_letter('o', 176, 28, 16, buffer, WIDTH);

  // draw_letter('t', 0, 46, 16, buffer, WIDTH);
  // draw_letter('s', 16, 46, 16, buffer, WIDTH);
  // draw_letter('+', 32, 46, 16, buffer, WIDTH);
  // draw_letter('-', 48, 46, 16, buffer, WIDTH);
  // draw_letter('*', 64, 46, 16, buffer, WIDTH);
  // draw_letter('/', 80, 46, 16, buffer, WIDTH);
  // draw_letter('^', 96, 46, 16, buffer, WIDTH);
  // draw_letter('(', 112, 46, 16, buffer, WIDTH);
  // draw_letter(')', 128, 46, 16, buffer, WIDTH);
  // draw_letter('=', 144, 46, 16, buffer, WIDTH);
  // draw_letter('.', 160, 46, 16, buffer, WIDTH);
  // draw_letter('?', 176, 46, 16, buffer, WIDTH);
  // draw_letter('x', 144, 46, 16, buffer, WIDTH);
  // draw_letter('y', 160, 46, 16, buffer, WIDTH);
  // draw_letter('w', 176, 46, 16, buffer, WIDTH);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(4 * WIDTH, 800, "Computer Algebra Solver");
  SetTargetFPS(30);

  // Initialize variables
  short cursor_pos = 0;

  Boolean second = FALSE;
  Boolean subscript = FALSE;

  char input_string[128] = {};
  int input_string_len = 0;

  struct Letter letter_buf;

  struct SolveVar input_var_buf[64] = {};
  int input_var_len = 0;

  double roots[64] = {};
  int roots_len = 0;

  enum StateMode mode = M_EXPRESSION;

  Boolean changed = FALSE;

  while (!WindowShouldClose()) {
    // Update
    // Update display
    if (changed && mode == M_EXPRESSION) {
      clear_display(buffer, SIZE);
      draw_expression(10, 44, 16, buffer, WIDTH, input_string, input_string_len,
                      &cursor_pos, FALSE, TRUE);
      draw_top_bar(WIDTH, buffer, 8, second, subscript);
      changed = FALSE;
    }

    // Draw
    BeginDrawing();

    ClearBackground(GRAY);

    // Draw display
    int scaling = GetRenderWidth() / WIDTH;
    for (int i = 0; i < SIZE; i++) {
      Color c = (Color){111, 153, 64, 255};
      if (buffer[i] != 0) {
        c = BLACK;
      }

      DrawRectangle((GetRenderWidth() - scaling * WIDTH) / 2.0 +
                        scaling * (i % WIDTH),
                    scaling * (i / WIDTH), scaling, scaling, c);
    }

    // Print input_string for debugging
    // input_string[input_string_len] = '\0';
    // DrawText(input_string, 20, 20, 48, BLACK);

    // Draw buttons
    int button_x_count = 6;
    int button_y_count = 8;
    int button_x = GetRenderWidth() / button_x_count;
    int button_y = (GetRenderHeight() - (HEIGHT * scaling)) / button_y_count;

    int m_x = GetMousePosition().x;
    int m_y = GetMousePosition().y;

    Boolean pressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    int row = 0;

    enum PushButton button_type = B_NONE;
    union PushButtonData button_data;

    if (draw_button("sin", 0 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_SIN;
    }
    if (draw_button("cos", 1 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_COS;
    }
    if (draw_button("tan", 2 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_TAN;
    }
    if (draw_button("DEL", 3 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_DEL;
    }
    if (draw_button("^", 4 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_UP;
    }
    if (draw_button("C", 5 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_CLEAR;
    }

    row = 1;
    if (draw_button("asin", 0 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_ASIN;
    }
    if (draw_button("acos", 1 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_ACOS;
    }
    if (draw_button("atan", 2 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_ATAN;
    }
    if (draw_button("<", 3 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_LEFT;
    }
    if (draw_button("v", 4 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_DOWN;
    }
    if (draw_button(">", 5 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_RIGHT;
    }

    row = 2;
    if (draw_button("sqrt", 0 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_SQRT;
    }
    if (draw_button("cbrt", 1 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_CBRT;
    }
    if (draw_button("nrt", 2 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_ROOT;
    }
    if (draw_button("START", 3 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_START;
    }
    if (draw_button("log", 4 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_LOG;
    }
    if (draw_button("2nd", 5 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_2ND;
    }

    row = 3;
    if (draw_button("^2", 0 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_SQR;
    }
    if (draw_button("^3", 1 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_CB;
    }
    if (draw_button("^n", 2 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_POW;
    }
    if (draw_button("END", 3 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_END;
    }
    if (draw_button("RST", 4 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      // Does nothing here, will be connected directly to rst pin of MCU
    }
    if (draw_button("SBSC", 5 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_SUBSCRIPT;
    }

    row = 4;
    if (draw_button("7", 0 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "a")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 7;
    }
    if (draw_button("8", 1 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "b")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 8;
    }
    if (draw_button("9", 2 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "c")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 9;
    }
    if (draw_button("EXPD", 3 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_EXPAND;
    }
    if (draw_button("=", 4 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_EQUAL;
    }
    if (draw_button("PI", 5 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_PI;
    }

    row = 5;
    if (draw_button("4", 0 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "d")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 4;
    }
    if (draw_button("5", 1 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "e")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 5;
    }
    if (draw_button("6", 2 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "f")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 6;
    }
    if (draw_button("RAR", 3 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_REARRANGE;
    }
    if (draw_button("(", 4 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_BLOCK_START;
    }
    if (draw_button(")", 5 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_BLOCK_END;
    }
    row = 6;
    if (draw_button("1", 0 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "x")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 1;
    }
    if (draw_button("2", 1 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "y")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 2;
    }
    if (draw_button("3", 2 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "w")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 3;
    }
    if (draw_button("GTRT", 3 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_GET_ROOT;
    }
    if (draw_button("*", 4 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_MULT;
    }
    if (draw_button("/", 5 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_DIV;
    }
    row = 7;
    if (draw_button("DEG", 0 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_DEG;
    }
    if (draw_button("0", 1 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_NUMBER_LETTER;
      button_data.number = 0;
    }
    if (draw_button(".", 2 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_DOT;
    }
    if (draw_button("SLV", 3 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_SOLVE;
    }
    if (draw_button("+", 4 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_PLUS;
    }
    if (draw_button("-", 5 * button_x, row * button_y + scaling * HEIGHT,
                    button_x, button_y, m_x, m_y, pressed, second, "null")) {
      button_type = B_MINUS;
    }

    if (pressed && button_type != B_NONE) {
      changed = TRUE;

      Boolean second_before = second;
      if (button_type == B_2ND) {
        second = !second;
      } else if (mode == M_EXPRESSION) {
        second = FALSE;
      }

      Boolean subscript_before = subscript;
      if (button_type == B_SUBSCRIPT) {
        subscript = TRUE;
        second = second_before;
      } else {
        subscript = FALSE;
      }

      f_bad_equation = FALSE;
      f_bad_term = FALSE;
      f_buffer_overflow = FALSE;
      f_overflow = FALSE;
      f_undefined = FALSE;
      f_unsupported = FALSE;
      if (button_type == B_CLEAR) {
        mode = M_EXPRESSION;
      }
      if (button_type == B_REARRANGE && mode != M_WHICH_VAR) {
        letter_buf.letter = ' ';
        letter_buf.subscript = ' ';
        mode = M_WHICH_VAR;
      }
      if (button_type == B_GET_ROOT && mode != M_SHOW_ROOTS) {
        struct EquationObject expression[192] = {};
        int new_len = lex(input_string, input_string_len, expression, 128);
        if (!valid_expr(expression, new_len)) {
          f_bad_equation = TRUE;
        } else {
          new_len = expand_polynomial(expression, new_len);
          if (!is_univariate(expression, new_len)) {
            f_bad_equation = TRUE;
          } else {
            mode = M_SHOW_ROOTS;
            for (int i = 0; i < new_len; i++) {
              if (expression[i].type == LETTER) {
                letter_buf = expression[i].value.letter;
                break;
              }
            }
            roots_len = real_roots(expression, new_len, roots);
            cursor_pos = 0;
          }
        }
      }
      if (button_type == B_SOLVE && mode != M_VAR_VALUE) {
        // Populate buffer
        struct EquationObject expression[192] = {};
        int new_len = lex(input_string, input_string_len, expression, 128);

        if (!valid_expr(expression, new_len)) {
          f_bad_equation = TRUE;
        } else {
          mode = M_VAR_VALUE;
          cursor_pos = 0;
          input_var_len = 0;
          for (int i = 0; i < new_len; i++) {
            if (expression[i].type == LETTER) {
              Boolean found = FALSE;
              for (int j = 0; j < input_var_len; j++) {
                struct Letter l = input_var_buf[input_string_len].letter;
                if (expression[i].value.letter.letter == l.letter &&
                    expression[i].value.letter.subscript == l.subscript) {
                  found = TRUE;
                  break;
                }
              }
              if (!found) {
                input_var_buf[input_var_len].letter =
                    expression[i].value.letter;
                input_var_buf[input_var_len].num_len = 0;
                input_var_len++;
              }
            }
          }
        }
      }

      switch (mode) {
      case M_EXPRESSION:
        if (input_string_len < 128 && button_type != B_GET_ROOT &&
            button_type != B_SOLVE && button_type != B_REARRANGE) {
          cursor_pos = button_update(input_string, &input_string_len,
                                     cursor_pos, button_type, button_data,
                                     second_before, subscript_before);
        }
        break;
      case M_WHICH_VAR: {
        clear_display(buffer, SIZE);
        Boolean finished = update_get_var(
            8, 16, buffer, WIDTH, &letter_buf, &subscript, &second, &mode,
            &cursor_pos, button_type, button_data);
        if (finished) {
          struct EquationObject expression[192] = {};
          int lex_len = lex(input_string, input_string_len, expression, 192);
          if (!valid_expr(expression, lex_len)) {
            f_bad_equation = TRUE;
            break;
          }
          int out_len = rearrange_for_var(expression, lex_len, letter_buf);
          out_len = 0;
          while (expression[out_len].type != END_LEX) {
            out_len++;
          }

          // Convert back to chars
          int buf_len = eo_to_string(expression, out_len, input_string);
          input_string_len = buf_len;
          cursor_pos = buf_len;
        }
        break;
      }
      case M_VAR_VALUE:
        clear_display(buffer, SIZE);
        update_get_var_values(8, 16, buffer, WIDTH, input_string,
                              &input_string_len, input_var_buf, &input_var_len,
                              &subscript, &mode, &cursor_pos, button_type,
                              button_data);
        break;
      case M_SHOW_ROOTS:
        clear_display(buffer, SIZE);
        update_show_roots(8, 16, buffer, WIDTH, roots, roots_len, &mode,
                          &cursor_pos, button_type, letter_buf, HEIGHT);
        break;
      }
    }

    EndDrawing();
  }

  CloseWindow();
}
