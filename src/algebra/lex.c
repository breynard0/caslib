#include "dutils.h"
#include "enums.h"
#include "equation_objects.h"
#include "pow.h"
#include "utils.h"

int lex(char* input, int length, struct EquationObject* buffer,
        int max_length)
{
    int i = 0;
    int out_len = 0;

    short state = 1;

    // struct EquationObject eo_def = { .type = NONE, .value.number = 0};
    // struct EquationObject eo = { .type = NONE, .value.none = 0};

    Boolean dot_flag = FALSE;
    short decimal_digits = 0;

    Boolean subscript = FALSE;

    while (i < length)
    {
        if (out_len + 3 >= max_length)
        {
            break;
        }

        char c = input[i];

        switch (state)
        {
        case 1:
            if (c >= '0' && c <= '9')
            {
                short num = c - '0';
                buffer[out_len].type = NUMBER;
                if (dot_flag == FALSE)
                {
                    buffer[out_len].value.number *= 10;
                    buffer[out_len].value.number += num;
                }
                else
                {
                    buffer[out_len].value.number +=
                        (double)num /
                        pow_di(10.0, -double_digits_partial(buffer[out_len].value.number));
                    decimal_digits++;
                }
                state = 2;
            }
            else if (c == '.')
            {
                dot_flag = TRUE;
                state = 2;
            }
            else if (c == ':')
            {
                subscript = TRUE;
                state = 3;
            }
            else if (c == '*')
            {
                buffer[out_len].type = MULT;
                out_len++;
                state = 1;
            }
            else if (c == '/')
            {
                buffer[out_len].type = DIV;
                out_len++;
                state = 1;
            }
            else if (c == '+')
            {
                buffer[out_len].type = ADD;
                out_len++;
                state = 1;
            }
            else if (c == '-')
            {
                buffer[out_len].type = SUB;
                out_len++;
                state = 1;
            }
            else if (c == '^')
            {
                buffer[out_len].type = EXP;
                out_len++;
                state = 1;
            }
            else if (c == '\\')
            {
                state = 4;
            }
            else if (c == '(')
            {
                buffer[out_len].type = BLOCK_START;
                out_len++;
                state = 1;
            }
            else if (c == ')')
            {
                buffer[out_len].type = BLOCK_END;
                out_len++;
                state = 1;
            }
            else if (c == '=')
            {
                buffer[out_len].type = EQUAL;
                out_len++;
                state = 1;
            }
            else
            {
                buffer[out_len].type = LETTER;

                // Default values
                buffer[out_len].value.letter.letter = ' ';
                buffer[out_len].value.letter.subscript = ' ';

                if (subscript == FALSE)
                {
                    buffer[out_len].value.letter.letter = c;
                }
                else
                {
                    buffer[out_len].value.letter.subscript = c;
                }
                state = 3;
            }
            break;
        case 2:
            if (c >= '0' && c <= '9')
            {
                short num = c - '0';
                buffer[out_len].type = NUMBER;
                if (dot_flag == FALSE)
                {
                    buffer[out_len].value.number *= 10;
                    buffer[out_len].value.number += num;
                }
                else
                {
                    if (decimal_digits < 12)
                    {
                        buffer[out_len].value.number =
                            dround(buffer[out_len].value.number * pow_di(10.0, decimal_digits)) /
                            pow_di(10.0, decimal_digits);
                        buffer[out_len].value.number += (double)num / pow_di(10.0, decimal_digits + 1);
                        buffer[out_len].value.number = round_to_threshold(buffer[out_len].value.number);
                        decimal_digits++;
                    }
                }
                state = 2;
            }
            else if (c == '.')
            {
                dot_flag = TRUE;
                state = 2;
            }
            else
            {
                dot_flag = FALSE;
                decimal_digits = 0;
                out_len++;

                i--;
                state = 1;
            }
            break;
        case 3:
            if (c == ':')
            {
                subscript = TRUE;
                state = 3;
            }
            else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                subscript)
            {
                buffer[out_len].type = LETTER;
                if ((!subscript && buffer[out_len].value.letter.letter != ' ') ||
                    (subscript && buffer[out_len].value.letter.subscript != ' '))
                {
                    subscript = FALSE;
                    out_len++;

                    i--;
                    state = 1;
                }
                else
                {
                    if (!subscript)
                    {
                        buffer[out_len].value.letter.letter = c;
                    }
                    else
                    {
                        buffer[out_len].value.letter.subscript = c;
                        subscript = FALSE;
                    }
                    state = 3;
                }
            }
            else
            {
                subscript = FALSE;
                out_len++;

                i--;
                state = 1;
            }
            break;
        case 4:
            switch (c)
            {
            case 'p':
                buffer[out_len].type = PI_VAL;
                out_len++;
                state = 1;
                break;
            case 's':
                buffer[out_len].type = SINE;
                out_len++;
                state = 1;
                break;
            case 'c':
                buffer[out_len].type = COSINE;
                out_len++;
                state = 1;
                break;
            case 't':
                buffer[out_len].type = TANGENT;
                out_len++;
                state = 1;
                break;
            case 'a':
                buffer[out_len].type = ARCSINE;
                out_len++;
                state = 1;
                break;
            case 'o':
                buffer[out_len].type = ARCCOSINE;
                out_len++;
                state = 1;
                break;
            case 'g':
                buffer[out_len].type = ARCTANGENT;
                out_len++;
                state = 1;
                break;
            case 'r':
                buffer[out_len].type = ROOT;
                out_len++;
                state = 1;
                break;
            case 'l':
                buffer[out_len].type = LOG;
                out_len++;
                state = 1;
                break;
            case 'd':
                if (buffer[out_len - 1].type == NUMBER)
                {
                    buffer[out_len - 1].value.number *= PI / 180.0;
                }
                break;
            default: break;
            }
            break;
        }
        i++;
    }

    // if (buffer[out_len].type != NONE)
    // {
    //     out_len++;
    // }
    buffer[out_len].type = END_LEX;
    out_len++;

    return out_len;
}