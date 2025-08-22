#include "buttons.h"
#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"
#include "flags.h"
#include "lex.h"
#include "valid.h"

void insert_char_idx(char* buffer, int length, char c, int idx)
{
    for (int i = length - 2; i >= idx; i--)
    {
        buffer[i + 1] = buffer[i];
    }
    buffer[idx] = c;
}

short button_update(char* buffer, int* length, short cursor_pos,
                    enum PushButton button, union PushButtonData data,
                    Boolean second, Boolean subscript)
{
    short cursor = cursor_pos;

    short colon_count = 0;
    for (int i = 0; i <= cursor; i++)
    {
        if (buffer[i] == ':')
        {
            colon_count++;
        }
    }

    // Make space for new thing
    if (cursor < *length &&
        !(button == B_SOLVE || button == B_REARRANGE || button == B_GET_ROOT ||
            button == B_EXPAND || button == B_SUBSCRIPT || button == B_2ND ||
            button == B_UP || button == B_DOWN || button == B_LEFT ||
            button == B_RIGHT || button == B_DEL || button == B_CLEAR ||
            button == B_START || button == B_END || button == B_NONE))
    {
        // Adjust for subscripts
        cursor += colon_count;

        for (int i = *length; i >= cursor; i--)
        {
            buffer[i] = buffer[i - 1];
        }
        buffer[cursor] = ' ';
    }
    if (cursor < *length &&
        (button == B_SQRT || button == B_CBRT || button == B_CB ||
            button == B_SQR || (button == B_NUMBER_LETTER && subscript)))
    {
        for (int i = *length + 1; i >= cursor; i--)
        {
            buffer[i] = buffer[i - 1];
        }
        buffer[cursor] = ' ';
    }

    // Expand functions
    if (button == B_SOLVE || button == B_REARRANGE || button == B_GET_ROOT ||
        button == B_EXPAND)
    {
        for (int i = 0; i < *length; i++)
        {
            Boolean found = FALSE;
            char c = buffer[i];
            if (c == '@')
            {
                buffer[i] = 'p';
                found = TRUE;
            }
            if (c == '#')
            {
                buffer[i] = 'r';
                found = TRUE;
            }
            if (c == '[')
            {
                buffer[i] = 's';
                found = TRUE;
            }
            if (c == ']')
            {
                buffer[i] = 'c';
                found = TRUE;
            }
            if (c == ';')
            {
                buffer[i] = 't';
                found = TRUE;
            }
            if (c == '`')
            {
                buffer[i] = 'a';
                found = TRUE;
            }
            if (c == '~')
            {
                buffer[i] = 'o';
                found = TRUE;
            }
            if (c == '$')
            {
                buffer[i] = 'g';
                found = TRUE;
            }
            if (c == '%')
            {
                buffer[i] = 'd';
                found = TRUE;
            }
            if (c == '&')
            {
                buffer[i] = 'l';
                found = TRUE;
            }
            if (found)
            {
                (*length)++;
                insert_char_idx(buffer, *length, '\\', i);
            }
        }
    }

    switch (button)
    {
    case B_SIN:
        buffer[cursor] = '[';
        (*length)++;
        cursor++;
        break;
    case B_COS:
        buffer[cursor] = ']';
        (*length)++;
        cursor++;
        break;
    case B_TAN:
        buffer[cursor] = ';';
        (*length)++;
        cursor++;
        break;
    case B_ASIN:
        buffer[cursor] = '`';
        (*length)++;
        cursor++;
        break;
    case B_ACOS:
        buffer[cursor] = '~';
        (*length)++;
        cursor++;
        break;
    case B_ATAN:
        buffer[cursor] = '$';
        (*length)++;
        cursor++;
        break;
    case B_SQRT:
        buffer[cursor] = '2';
        (*length)++;
        cursor++;
        buffer[cursor] = '#';
        (*length)++;
        cursor++;
        break;
    case B_CBRT:
        buffer[cursor] = '3';
        (*length)++;
        cursor++;
        buffer[cursor] = '#';
        (*length)++;
        cursor++;
        break;
    case B_ROOT:
        buffer[cursor] = '#';
        (*length)++;
        cursor++;
        break;
    case B_SQR:
        buffer[cursor] = '^';
        (*length)++;
        cursor++;
        buffer[cursor] = '2';
        (*length)++;
        cursor++;
        break;
    case B_CB:
        buffer[cursor] = '^';
        (*length)++;
        cursor++;
        buffer[cursor] = '3';
        (*length)++;
        cursor++;
        break;
    case B_POW:
        buffer[cursor] = '^';
        (*length)++;
        cursor++;
        break;
    case B_LOG:
        buffer[cursor] = '&';
        (*length)++;
        cursor++;
        break;
    case B_NUMBER_LETTER:
        if (subscript)
        {
            buffer[cursor] = ':';
            (*length)++;
            cursor++;
        }

        if (second)
        {
            char letter = 'a';
            switch (data.number)
            {
            case 7:
                letter = 'a';
                break;
            case 8:
                letter = 'b';
                break;
            case 9:
                letter = 'c';
                break;
            case 4:
                letter = 'd';
                break;
            case 5:
                letter = 'e';
                break;
            case 6:
                letter = 'f';
                break;
            case 1:
                letter = 'x';
                break;
            case 2:
                letter = 'y';
                break;
            case 3:
                letter = 'w';
                break;
            }
            buffer[cursor] = letter;
        }
        else
        {
            buffer[cursor] = data.number + '0';
        }
        (*length)++;
        if (!subscript)
        {
            cursor++;
        }

        break;
    case B_PLUS:
        buffer[cursor] = '+';
        (*length)++;
        cursor++;
        break;
    case B_MINUS:
        buffer[cursor] = '-';
        (*length)++;
        cursor++;
        break;
    case B_MULT:
        buffer[cursor] = '*';
        (*length)++;
        cursor++;
        break;
    case B_DIV:
        buffer[cursor] = '/';
        (*length)++;
        cursor++;
        break;
    case B_BLOCK_START:
        buffer[cursor] = '(';
        (*length)++;
        cursor++;
        break;
    case B_BLOCK_END:
        buffer[cursor] = ')';
        (*length)++;
        cursor++;
        break;
    case B_EQUAL:
        buffer[cursor] = '=';
        (*length)++;
        cursor++;
        break;
    case B_EXPAND:
        {
            // This is larger than the character buffer to handle juxtposition, etc.
            struct EquationObject expression[192] = {};
            int lex_len = lex(buffer, *length, expression, 192);
            if (!valid_expr(expression, lex_len))
            {
                f_bad_equation = TRUE;
                break;
            }
            if (!no_var_functions(expression, lex_len))
            {
                f_unsupported = TRUE;
                break;
            }

            expand_polynomial(expression, lex_len);
            int out_len = 0;
            while (expression[out_len].type != END_LEX)
            {
                out_len++;
            }

            // Convert back to chars
            int buf_len = eo_to_string(expression, out_len, buffer);
            *length = buf_len;
            cursor = 0;
        }
        break;
    case B_PI:
        buffer[cursor] = '@';
        (*length)++;
        cursor++;
        break;
    case B_UP:
        {
            if (cursor > 0)
            {
                cursor--;
                while (cursor > 0 && buffer[cursor] != '(' && buffer[cursor] != ')')
                {
                    cursor--;
                }
            }
            break;
        }
    case B_DOWN:
        if (cursor < *length)
        {
            cursor++;
            while (cursor < *length && buffer[cursor] != '(' &&
                buffer[cursor] != ')')
            {
                cursor++;
            }
        }
        break;
    case B_LEFT:
        if (cursor > 0)
        {
            cursor--;
        }
        break;
    case B_RIGHT:
        {
            int num_subscript = 0;
            for (int i = 0; i < *length; i++)
            {
                if (buffer[i] == ':')
                {
                    num_subscript++;
                }
            }

            if (cursor < (*length - num_subscript))
            {
                cursor++;
            }
            break;
        }
    case B_DEL:
        {
            if (*length == 0)
            {
                return cursor;
            }

            int del_pos = cursor - 1;
            if (del_pos < 0)
            {
                del_pos = 0;
            }

            char del_char = buffer[del_pos];

            Boolean cursor_at_end = TRUE;
            if (cursor != *length)
            {
                cursor_at_end = FALSE;
                for (int i = del_pos; i < *length - 1; i++)
                {
                    buffer[i] = buffer[i + 1];
                }
            }
            if (length >= 0)
            {
                (*length)--;
                if (cursor_at_end)
                {
                    cursor = *length;
                }
                else
                {
                    cursor--;
                }
            }
            if (del_char == ':')
            {
                button_update(buffer, length, del_pos + 1, B_DEL, data, second,
                              subscript);
            }
            break;
        }
    case B_CLEAR:
        *length = 0;
        cursor = 0;
        buffer[0] = '\0';
        break;
    case B_DEG:
        buffer[*length] = '%';
        (*length)++;
        cursor++;
        break;
    case B_DOT:
        buffer[*length] = '.';
        (*length)++;
        cursor++;
        break;
    case B_START:
        cursor = 0;
        break;
    case B_END:
        cursor = *length;
        break;
    case B_SUBSCRIPT:
    case B_2ND:
    case B_NONE:
    case B_SOLVE:
    case B_REARRANGE:
    case B_GET_ROOT:
        break;
    }

    if (cursor > *length - colon_count)
    {
        cursor = *length - colon_count;
    }

    return cursor;
}