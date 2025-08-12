#include "draw.h"
#include "enums.h"
#include "equation_objects.h"

void update_get_var(short y_spacer, short size, char *buffer, short buf_width,
                    struct Letter *letter_buf, Boolean *subscript,
                    enum StateMode *mode, short *cursor);
void update_get_var_values(short y_spacer, short size, char *buffer,
                           short buf_width, char *expression_in, short expr_len,
                           double *values_buf, int *values_len,
                           Boolean *subscript, enum StateMode *mode,
                           short *cursor);
void update_show_roots(short y_spacer, short size, char *buffer,
                       short buf_width, double *roots, int roots_len,
                       Boolean *subscript, enum StateMode *mode, short *cursor);