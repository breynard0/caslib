#include "buttons.h"
#include "draw.h"
#include "enums.h"
#include "equation_objects.h"
#include "parse.h"

Boolean update_get_var(short y_spacer, short size, char* buffer,
                       short buf_width, struct Letter* letter_buf,
                       Boolean* subscript, Boolean* second,
                       enum StateMode* mode, short* cursor,
                       enum PushButton button, union PushButtonData data,
                       char* input_string, int input_string_len);
void update_get_var_values(short y_spacer, short size, char* buffer,
                           short buf_width, char* expression_in, int* expr_len,
                           struct SolveVar* values_buf, int* values_len,
                           Boolean* subscript, enum StateMode* mode,
                           short* cursor, enum PushButton button,
                           union PushButtonData data);
void update_show_roots(short y_spacer, short size, char* buffer,
                       short buf_width, double* roots, int roots_len,
                       enum StateMode* mode, short* cursor,
                       enum PushButton button, struct Letter letter,
                       short height);