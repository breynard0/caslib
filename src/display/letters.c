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
  draw_line(data.x, data.y, data.x, data.y + data.height,
            buffer, data.buf_width);
  draw_line(data.x, data.y + data.height, data.x + data.width,
            data.y + data.height, buffer, data.buf_width);
  draw_line(data.x + data.width, data.y, data.x + data.width,
            data.y + data.height, buffer, data.buf_width);
  draw_line(data.x, data.y, data.x + data.width, data.y, buffer,
            data.buf_width);
  set_pixel(data.x + data.width, data.y + data.height, ON, buffer,
            data.buf_width);
}

void draw1(struct DrawData data, char *buffer) {
  // Stem
  draw_line(data.x + data.width / 2, data.y, data.x + data.width / 2, data.y + data.height + 1, buffer, data.buf_width);
  // Serif
  draw_line(data.x + data.width / 2, data.y, data.x + (short)((float)data.width / 2.5), data.y, buffer, data.buf_width);
}

void draw2(struct DrawData data, char *buffer) {
  // Top
  draw_line(data.x, data.y, data.x + data.width, data.y, buffer, data.buf_width);
  // Top vert
  draw_line(data.x + data.width, data.y, data.x + data.width, data.y + data.height / 2, buffer, data.buf_width);
  // Middle
  draw_line(data.x, data.y + data.height / 2, data.x + data.width + 1, data.y + data.height / 2, buffer, data.buf_width);
  // Bottom vert
  draw_line(data.x, data.y + data.height / 2, data.x, data.y + data.height, buffer, data.buf_width);
  // Bottom
  draw_line(data.x, data.y + data.height, data.x + data.width, data.y + data.height, buffer, data.buf_width);
}

void draw3(struct DrawData data, char *buffer) {
  // Stem
  draw_line(data.x + data.width, data.y, data.x + data.width, data.y + data.height + 1, buffer, data.buf_width);
  // Top
  draw_line(data.x, data.y, data.x + data.width, data.y, buffer, data.buf_width);
  // Middle
  draw_line(data.x, data.y + data.height / 2, data.x + data.width, data.y + data.height / 2, buffer, data.buf_width);
  // Bottom
  draw_line(data.x, data.y + data.height, data.x + data.width, data.y + data.height, buffer, data.buf_width);
}

void draw4(struct DrawData data, char *buffer) {
  // Stem
  draw_line(data.x + data.width, data.y, data.x + data.width, data.y + data.height + 1, buffer, data.buf_width);
  // Vert
  draw_line(data.x + data.width / 4, data.y, data.x + data.width / 4, data.y + data.height / 2, buffer, data.buf_width);
  // Middle
  draw_line(data.x + data.width / 4, data.y + data.height / 2, data.x + data.width, data.y + data.height / 2, buffer, data.buf_width);
}

void draw5(struct DrawData data, char *buffer) {
  // Top
  draw_line(data.x, data.y, data.x + data.width + 1, data.y, buffer, data.buf_width);
  // Top vert
  draw_line(data.x, data.y, data.x, data.y + data.height / 2, buffer, data.buf_width);
  // Middle
  draw_line(data.x, data.y + data.height / 2, data.x + data.width + 1, data.y + data.height / 2, buffer, data.buf_width);
  // Bottom vert
  draw_line(data.x + data.width, data.y + data.height / 2, data.x + data.width, data.y + data.height, buffer, data.buf_width);
  // Bottom
  draw_line(data.x, data.y + data.height, data.x + data.width + 1, data.y + data.height, buffer, data.buf_width);
}

void draw6(struct DrawData data, char *buffer) {
  // Stem
  draw_line(data.x + data.width, data.y, data.x + data.width, data.y + data.height + 1, buffer, data.buf_width);
  // Vert
  draw_line(data.x + data.width / 2, data.y + data.height / 2, data.x + data.width / 2, data.y + data.height, buffer, data.buf_width);
  // Middle
  draw_line(data.x + data.width / 2, data.y + data.height / 2, data.x + data.width, data.y + data.height / 2, buffer, data.buf_width);
  // Bottom
  draw_line(data.x + data.width / 2, data.y + data.height, data.x + data.width, data.y + data.height, buffer, data.buf_width);
}

void draw7(struct DrawData data, char *buffer) {
  // Stem
  draw_line(data.x + data.width, data.y, data.x + data.width, data.y + data.height + 1, buffer, data.buf_width);
  // Serif
  draw_line(data.x + data.width, data.y, data.x + (short)((float)data.width / 2.5), data.y, buffer, data.buf_width);
}

void draw8(struct DrawData data, char *buffer) {
  // Right Stem
  draw_line(data.x + data.width, data.y, data.x + data.width, data.y + data.height + 1, buffer, data.buf_width);
  // Left Stem
  draw_line(data.x, data.y, data.x, data.y + data.height + 1, buffer, data.buf_width);
  // Top
  draw_line(data.x, data.y, data.x + data.width, data.y, buffer, data.buf_width);
  // Middle
  draw_line(data.x, data.y + data.height / 2, data.x + data.width, data.y + data.height / 2, buffer, data.buf_width);
  // Bottom
  draw_line(data.x, data.y + data.height, data.x + data.width, data.y + data.height, buffer, data.buf_width);
}

void draw9(struct DrawData data, char *buffer) {
  // Stem
  draw_line(data.x + data.width, data.y, data.x + data.width, data.y + data.height + 1, buffer, data.buf_width);
  // Vert
  draw_line(data.x + data.width / 2, data.y, data.x + data.width / 2, data.y + data.height / 2, buffer, data.buf_width);
  // Middle
  draw_line(data.x + data.width / 2, data.y + data.height / 2, data.x + data.width, data.y + data.height / 2, buffer, data.buf_width);
  // Top
  draw_line(data.x + data.width / 2, data.y, data.x + data.width, data.y, buffer, data.buf_width);
}

void draw_letter(char letter, short x, short y, short width, short height,
                 char *buffer, short buf_width) {
  struct DrawData data = {};
  data.x = x;
  data.y = y;
  data.width = width;
  data.height = height;
  data.buf_width = buf_width;

  // 4 tall per 3 wide
  scale_ratio(&data.width, &data.height, 4, 3);

  switch (letter) {
  case '0':
    draw0(data, buffer);
    break;
  case '1':
    draw1(data, buffer);
    break;
  case '2':
    draw2(data, buffer);
    break;
  case '3':
    draw3(data, buffer);
    break;
  case '4':
    draw4(data, buffer);
    break;
  case '5':
    draw5(data, buffer);
    break;
  case '6':
    draw6(data, buffer);
    break;
  case '7':
    draw7(data, buffer);
    break;
  case '8':
    draw8(data, buffer);
    break;
  case '9':
    draw9(data, buffer);
    break;
  }
}
