#ifndef DRAW_STUFF
#define DRAW_STUFF

enum PixelState { OFF, ON };

enum StateMode { M_EXPRESSION, M_WHICH_VAR, M_VAR_VALUE, M_SHOW_ROOTS };
#endif
#include "enums.h"

void clear_display(char* buffer, short length);
void set_pixel(short x, short y, short value, char* buffer, short width);
void set_pixel_on(short x, short y, char* buffer, short width);
void draw_line(short x0, short y0, short x1, short y1, char* buffer,
               short width);
void draw_rect(short x, short y, short width, short height, char* buffer,
               short buf_width);
// Returns position of the draw cursor at the end
int draw_expression(short x, short y, short size, char* buffer, short buf_width,
                    char* expression_in, short length, short* cursor,
                    Boolean cursor_calculation, Boolean draw_cursor);

int draw_terms(short x, short y, short size, char* buffer, short buf_width, char* expression_in, short length,
               short* cursor, Boolean cursor_calculation, Boolean draw_cursor);

void draw_top_bar(short width, char* buffer, short size, Boolean second,
                  Boolean ssc);
