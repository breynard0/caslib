#ifndef DRAW_STUFF
#define DRAW_STUFF
enum PixelState { OFF, ON };
#endif

void clear_display(char *buffer, short length);
void set_pixel(short x, short y, short value, char *buffer, short width);
void set_pixel_on(short x, short y, char *buffer, short width);
void draw_line(short x0, short y0, short x1, short y1, char *buffer,
               short width);
void draw_rect(short x, short y, short width, short height, char *buffer,
               short buf_width);
void draw_expression(short x, short y, short size, char *buffer,
                     short buf_width, char *expression, short length,
                     short cursor);