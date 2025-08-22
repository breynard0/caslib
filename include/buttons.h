#include "enums.h"
#include "equation_objects.h"

#ifndef ButtonsValues
#define ButtonsValues

enum PushButton
{
    B_SIN,
    B_COS,
    B_TAN,
    B_ASIN,
    B_ACOS,
    B_ATAN,
    B_SQRT,
    B_CBRT,
    B_ROOT,
    B_SQR,
    B_CB,
    B_POW,
    B_LOG,
    B_NUMBER_LETTER,
    B_PLUS,
    B_MINUS,
    B_MULT,
    B_DIV,
    B_BLOCK_START,
    B_BLOCK_END,
    B_EQUAL,
    B_SOLVE,
    B_REARRANGE,
    B_GET_ROOT,
    B_EXPAND,
    B_SUBSCRIPT,
    B_2ND,
    B_PI,
    B_UP,
    B_DOWN,
    B_LEFT,
    B_RIGHT,
    B_DEL,
    B_CLEAR,
    B_DEG,
    B_DOT,
    B_START,
    B_END,
    B_NONE,
};

union PushButtonData
{
    int number;
};

struct SolveVar
{
    struct Letter letter;
    char num[25];
    short num_len;
};
#endif

// Returns new cursor position
short button_update(char* buffer, int* length, short cursor_pos,
                    enum PushButton button, union PushButtonData data,
                    Boolean second, Boolean capital);