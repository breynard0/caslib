#include "atrig.h"
#include "enums.h"
#include "equation_objects.h"
#include "log.h"
#include "pow.h"
#include "root.h"
#include "solve_consts.h"
#include "trig.h"
#include "utils.h"

int evaluate_consts(struct EquationObject* expression, int length)
{
    // struct EquationObject expression[length];
    // for (int i = 0; i < length; i++) {
    //   expression[i] = expressione[i];
    // }
    int new_len = length;

    // Evaluate constant blocks
    int i = 0;
    while (i < new_len)
    {
        struct EquationObject cur = expression[i];

        if (cur.type == BLOCK_END)
        {
            int start = i - 1;
            int count = 1;
            Boolean vars = FALSE;

            while (count != 0)
            {
                if (expression[start].type == BLOCK_START)
                {
                    count--;
                }
                else if (expression[start].type == BLOCK_END)
                {
                    count++;
                }
                else if (expression[start].type == LETTER)
                {
                    vars = TRUE;
                }
                if (count != 0)
                {
                    start--;
                }
            }

            if (!vars)
            {
                struct EquationObject tmp_buf[i - start - 1];
                for (int j = 1; j < i - start; j++)
                {
                    tmp_buf[j - 1] = expression[start + j];
                }

                expression[start].type = NUMBER;
                expression[start].value.number =
                    solve_const_expr(tmp_buf, i - start - 1, 0, 0);
                for (int j = 0; j < i - start; j++)
                {
                    remove_eo_idx(expression, new_len, start + 1);
                    new_len--;
                }
                i = start;
            }
        }

        i++;
    }

    // Evaluate functions
    i = 0;
    while (i < new_len)
    {
        struct EquationObject cur = expression[i];

        switch (cur.type)
        {
        case PI_VAL:
            expression[i].type = NUMBER;
            expression[i].value.number = PI;
            i = 0;
            break;
        case ROOT:
            expression[i].type = NUMBER;
            expression[i].value.number = nth_root(expression[i - 1].value.number,
                                                  expression[i + 1].value.number);
            remove_eo_idx(expression, new_len, i + 1);
            new_len--;
            remove_eo_idx(expression, new_len, i - 1);
            new_len--;
            i = 0;
            break;
        case EXP:
            if (expression[i - 1].type == NUMBER)
            {
                expression[i].type = NUMBER;
                expression[i].value.number = pow_dd(expression[i - 1].value.number,
                                                    expression[i + 1].value.number);
                remove_eo_idx(expression, new_len, i + 1);
                new_len--;
                remove_eo_idx(expression, new_len, i - 1);
                new_len--;
                i = 0;
            }
            break;
        case SINE:
            expression[i].type = NUMBER;
            expression[i].value.number = sine(expression[i + 1].value.number);
            remove_eo_idx(expression, new_len, i + 1);
            new_len--;
            i = 0;
            break;
        case COSINE:
            expression[i].type = NUMBER;
            expression[i].value.number = cosine(expression[i + 1].value.number);
            remove_eo_idx(expression, new_len, i + 1);
            new_len--;
            i = 0;
            break;
        case TANGENT:
            expression[i].type = NUMBER;
            expression[i].value.number = tangent(expression[i + 1].value.number);
            remove_eo_idx(expression, new_len, i + 1);
            new_len--;
            i = 0;
            break;
        case ARCSINE:
            expression[i].type = NUMBER;
            expression[i].value.number = arc_sine(expression[i + 1].value.number);
            remove_eo_idx(expression, new_len, i + 1);
            new_len--;
            i = 0;
            break;
        case ARCCOSINE:
            expression[i].type = NUMBER;
            expression[i].value.number = arc_cosine(expression[i + 1].value.number);
            remove_eo_idx(expression, new_len, i + 1);
            new_len--;
            i = 0;
            break;
        case ARCTANGENT:
            expression[i].type = NUMBER;
            expression[i].value.number = arc_tangent(expression[i + 1].value.number);
            remove_eo_idx(expression, new_len, i + 1);
            new_len--;
            i = 0;
            break;
        case LOG:
            expression[i].type = NUMBER;
            expression[i].value.number =
                log_n(expression[i - 1].value.number, expression[i + 1].value.number);
            remove_eo_idx(expression, new_len, i + 1);
            new_len--;
            remove_eo_idx(expression, new_len, i - 1);
            new_len--;
            i = 0;
            break;
        default:
            break;
        }

        i++;
    }

    return new_len;
}