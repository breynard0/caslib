#include "draw.h"

struct DrawData {
  short x;
  short y;
  short width;
  short height;
  short buf_width;
};

void draw0(struct DrawData data, char* buffer) {
  draw_line(data.x + 1, data.y + 1, data.x + 1, data.y + data.height - 1, buffer, data.buf_width);
  draw_line(data.x + 1, data.y + data.height - 1, data.x +  data.width - 1, data.y + data.height - 1, buffer, data.buf_width);
  draw_line(data.x + data.width - 1, data.y + 1, data.x + data.width - 1, data.y + data.height - 1, buffer, data.buf_width);
  draw_line(data.x + 1, data.y + 1, data.x + data.width - 1, data.y + 1, buffer, data.buf_width);
  set_pixel(data.x + data.width - 1, data.y + data.height - 1, ON, buffer, data.buf_width);
}

void draw_letter(char letter, short x, short y, short width, short height,
                 char *buffer, short buf_width) {
  struct DrawData data = {};
  data.x = x;
  data.y = y;
  data.width = width;
  data.height = height;
  data.buf_width = buf_width;

  switch (letter) {
  case '0':
    draw0(data, buffer);
    break;
  }
}
