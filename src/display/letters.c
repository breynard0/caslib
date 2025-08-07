#include "draw.h"
#include "dutils.h"

struct DrawData {
  short x;
  short y;
  short width;
  short height;
  short buf_width;
};

void scale_ratio(short *width, short *height, short h_ratio, short w_ratio) {
  float w_float = (float)*width;
  float h_float = (float)*height;
  float hr_float = (float)h_ratio;
  float wr_float = (float)w_ratio;
  float ratio = hr_float / wr_float;

  if (w_float > h_float) {
    w_float = h_float / ratio;
    *width = (short)dround(w_float);
  } else {
    h_float = w_float * ratio;
    *height = (short)dround(h_float);
  }
}

void draw0(struct DrawData data, char *buffer) {
  // 4 tall per 3 wide
  scale_ratio(&data.width, &data.height, 4, 3);

  draw_line(data.x + 1, data.y + 1, data.x + 1, data.y + data.height - 1,
            buffer, data.buf_width);
  draw_line(data.x + 1, data.y + data.height - 1, data.x + data.width - 1,
            data.y + data.height - 1, buffer, data.buf_width);
  draw_line(data.x + data.width - 1, data.y + 1, data.x + data.width - 1,
            data.y + data.height - 1, buffer, data.buf_width);
  draw_line(data.x + 1, data.y + 1, data.x + data.width - 1, data.y + 1, buffer,
            data.buf_width);
  set_pixel(data.x + data.width - 1, data.y + data.height - 1, ON, buffer,
            data.buf_width);
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
