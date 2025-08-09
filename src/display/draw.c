#include "draw.h"
#include "buttons.h"
#include "dutils.h"
#include "enums.h"
#include "gcf.h"
#include "letters.h"

void clear_display(char *buffer, short length) {
  for (int i = 0; i < length; i++) {
    buffer[i] = OFF;
  }
}

void set_pixel(short x, short y, short value, char *buffer, short width) {
  if (x < 0 || x >= width || y < 0) {
    return;
  }
  buffer[y * width + x] = value;
}

void set_pixel_on(short x, short y, char *buffer, short width) {
  if (x < 0 || x >= width || y < 0) {
    return;
  }
  buffer[y * width + x] = ON;
}

int i_min(int a, int b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}
int i_max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

void draw_line(short x0, short y0, short x1, short y1, char *buffer,
               short width) {
  short higher_y = y0;
  short lower_y = y1;
  if (y1 > y0) {
    higher_y = y1;
    lower_y = y0;
  }
  short higher_x = x0;
  short lower_x = x1;
  if (x1 > x0) {
    higher_x = x1;
    lower_x = x0;
  }
  short rise = higher_y - lower_y;
  short run = higher_x - lower_x;
  int max = rise;
  if (run > rise) {
    max = run;
  }

  if (x0 == x1) {
    short higher = y0;
    short lower = y1;
    if (y1 > y0) {
      higher = y1;
      lower = y0;
    }
    for (int i = lower; i < higher; i++) {
      set_pixel(x0, i, ON, buffer, width);
    }
    return;
  }

  if (y0 == y1) {
    short higher = x0;
    short lower = x1;
    if (x1 > x0) {
      higher = x1;
      lower = x0;
    }
    for (int i = lower; i < higher; i++) {
      set_pixel(i, y0, ON, buffer, width);
    }
    return;
  }

  if (run == rise) {
    short higher = x0;
    short lower = x1;
    if (x1 > x0) {
      higher = x1;
      lower = x0;
    }
    for (int i = lower; i <= higher; i++) {
      set_pixel(i, i, ON, buffer, width);
    }
    return;
  }

  float f_rise = (float)rise;
  float f_run = (float)run;

  if (run > rise) {
    double length = f_run / (f_rise + 1);

    for (int i = 0; i <= higher_y - lower_y; i++) {
      i = i;
      for (int j = -1; j < length; j++) {
        set_pixel(lower_x + (i * length) + j, lower_y + i, ON, buffer, width);
      }
    }
  } else {
    double length = f_rise / (f_run + 1);

    for (int i = 0; i <= higher_x - lower_x; i++) {
      for (int j = 0; j < length; j++) {
        set_pixel(lower_x + i, lower_y + (i + length) + j, ON, buffer, width);
      }
    }
  }
}

void draw_rect(short x, short y, short width, short height, char *buffer,
               short buf_width) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      set_pixel(x + j, y + i, ON, buffer, buf_width);
    }
  }
}

int draw_expression(short x, short y, short size, char *buffer, short buf_width,
                    char *expression_in, short length, short *cursor,
                    Boolean cursor_calculation) {
  char expression[length] = {};
  int new_len = 0;
  // Get all subscripts
  short subscripts[length] = {};
  short sub_len = 0;
  for (int i = 0; i < length; i++) {
    if (expression_in[i] == ':') {
      subscripts[sub_len] = i;
      sub_len++;
    } else {
      expression[new_len] = expression_in[i];
      new_len++;
    }
  }

  // Set offset
  short offset = 0;
  if (!cursor_calculation) {
    short cursor_pos = draw_expression(x, y, size, buffer, buf_width,
                                       expression_in, length, cursor, TRUE);
    // x -= i_max(cursor_pos - buf_width, 0);
    if (cursor_pos >= buf_width) {
      offset = -i_max(cursor_pos - buf_width + buf_width / 4, 0);
    }
  }

  int i = 0;
  int draw_spot = 0;
  while (i < new_len) {
    int draw_spot_before = draw_spot;

    char cur_let = expression[i];

    if (cur_let == '\\') {
      i++;
    }

    Boolean is_subscript = FALSE;
    for (int j = 0; j < sub_len; j++) {
      if (i == subscripts[j]) {
        is_subscript = TRUE;
        if (!cursor_calculation) {
          draw_letter('0', x + draw_spot + offset, y + (size - 8), size - 8,
                      buffer, buf_width);
        }
        draw_spot += size - 8;
      }
    }

    if (!is_subscript) {
      switch (cur_let) {
      // Sine
      case '[': {
        short cumul_draw_spot = 0;
        if (!cursor_calculation) {
          draw_letter('s', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 4);
        if (!cursor_calculation) {
          draw_letter('i', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 4);
        if (!cursor_calculation) {
          draw_letter('n', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size;
        draw_spot += cumul_draw_spot;
        break;
      }
      // Cosine
      case ']': {
        short cumul_draw_spot = 0;
        if (!cursor_calculation) {
          draw_letter('c', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 8);
        if (!cursor_calculation) {
          draw_letter('o', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 8);
        if (!cursor_calculation) {
          draw_letter('s', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size;
        draw_spot += cumul_draw_spot;
        break;
      }
      // Tangent
      case ';': {
        short cumul_draw_spot = 0;
        if (!cursor_calculation) {
          draw_letter('t', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 4);
        if (!cursor_calculation) {
          draw_letter('a', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 8);
        if (!cursor_calculation) {
          draw_letter('n', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size;
        draw_spot += cumul_draw_spot;
        break;
      }
      // Arc Sine
      case '`': {
        short cumul_draw_spot = 0;
        if (!cursor_calculation) {
          draw_letter('a', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 8);
        if (!cursor_calculation) {
          draw_letter('s', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 4);
        if (!cursor_calculation) {
          draw_letter('i', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 4);
        if (!cursor_calculation) {
          draw_letter('n', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size;
        draw_spot += cumul_draw_spot;
        break;
      }
      // Arc Cosine
      case '~': {
        short cumul_draw_spot = 0;
        if (!cursor_calculation) {
          draw_letter('a', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 8);
        if (!cursor_calculation) {
          draw_letter('c', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 8);
        if (!cursor_calculation) {
          draw_letter('o', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 8);
        if (!cursor_calculation) {
          draw_letter('s', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size;
        draw_spot += cumul_draw_spot;
        break;
      }
      // Arc Tangent
      case '$': {
        short cumul_draw_spot = 0;
        if (!cursor_calculation) {
          draw_letter('a', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 8);
        if (!cursor_calculation) {
          draw_letter('t', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 4);
        if (!cursor_calculation) {
          draw_letter('a', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 8);
        if (!cursor_calculation) {
          draw_letter('n', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size;
        draw_spot += cumul_draw_spot;
        break;
      }
      // Log
      case '&': {
        short cumul_draw_spot = 0;
        if (!cursor_calculation) {
          draw_letter('l', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 6);
        if (!cursor_calculation) {
          draw_letter('o', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size - (size / 6);
        if (!cursor_calculation) {
          draw_letter('g', x + cumul_draw_spot + draw_spot + offset, y, size,
                      buffer, buf_width);
        }
        cumul_draw_spot += size;
        draw_spot += cumul_draw_spot;
        break;
      }
      // Dot
      case '.': {
        draw_spot -= (size / 3);
        if (!cursor_calculation) {
          draw_letter(expression[i], x + draw_spot + offset, y, size, buffer,
                      buf_width);
        }
        draw_spot += 2 * (size / 3);
        break;
      }
      default:
        if (!cursor_calculation) {
          draw_letter(expression[i], x + draw_spot + offset, y, size, buffer,
                      buf_width);
        }
        draw_spot += size;
        break;
      }
    }

    // Draw cursor
    if (*cursor == i) {
      if (!cursor_calculation) {
        draw_line(draw_spot_before + x + offset, y - 2,
                  draw_spot_before + x + offset, y + size + 2, buffer,
                  buf_width);
      } else {
        return draw_spot_before + x + offset;
      }
    }

    i++;
  }

  // Draw cursor at end
  if (!cursor_calculation) {
    if (*cursor == new_len || new_len == 0) {
      draw_line(draw_spot + x + offset, y + size + 2,
                draw_spot + x + offset + size, y + size + 2, buffer, buf_width);
    }
  }

  return x + draw_spot + offset;
}