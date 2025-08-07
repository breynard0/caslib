#include "draw.h"
#include "dutils.h"
#include "gcf.h"

void clear_display(char *buffer, short length) {
  for (int i = 0; i < length; i++) {
    buffer[i] = OFF;
  }
}

void set_pixel(short x, short y, short value, char *buffer, short width) {
  buffer[y * width + x] = value;
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
  short rise = y1 - y0;
  short run = x1 - x0;
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

  if (run > rise) {
    double length = f_run / f_rise;

    for (int i = 0; i < higher_y - lower_y; i++) {
      i = i;
      for (int j = -1; j < length; j++) {
        set_pixel(lower_x + (i * length) + j, lower_y + i, ON, buffer, width);
      }
    }
  } else {
    double length = f_rise / f_run;

    for (int i = 0; i < higher_x - lower_x; i++) {
      for (int j = -1; j < length; j++) {
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