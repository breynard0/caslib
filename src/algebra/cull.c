#include "equation_objects.h"

int cull_the_useless(struct EquationObject* expression, int length)
{
    struct EquationObject buf[length] = {};
    for (int i = 0; i < length; i++)
    {
        buf[i] = expression[i];
    }
    int new_len = length;
    int i = 1;
    while (i < new_len)
    {
        // x^0
        if (buf[i].type == NUMBER && buf[i].value.number == 0 &&
            buf[i - 1].type == EXP && buf[i - 2].type == LETTER)
        {
            int before = i - 3;
            if (before >= 0 &&
                (buf[before].type == ADD || buf[before].type == SUB ||
                    buf[before].type == MULT || buf[before].type == DIV))
            {
                remove_eo_idx(buf, new_len, before);
                new_len--;
                i--;
            }
            else
            {
                // Remove sign
                int after = i + 1;
                if (buf[after].type == ADD || buf[after].type == SUB ||
                    buf[after].type == MULT || buf[after].type == DIV)
                {
                    remove_eo_idx(buf, new_len, after);
                    new_len--;
                    if (buf[after].type == SUB)
                    {
                        struct EquationObject obj;
                        obj.type = MULT;
                        insert_eo_idx(buf, new_len, after, obj);
                        obj.type = NUMBER;
                        obj.value.number = -1;
                        insert_eo_idx(buf, new_len, after, obj);
                    }
                }
            }
            // Remove variable
            for (int j = 0; j < 3; j++)
            {
                remove_eo_idx(buf, new_len, i);
                i--;
                new_len--;
            }
        }
        i++;
    }

    i = 0;
    while (i < new_len)
    {
        // 1*
        if (i != 0 && ((buf[i].type == NUMBER && buf[i].value.number == 1 &&
                buf[i - 1].type == MULT) ||
            (buf[i].type == MULT && buf[i - 1].type == NUMBER &&
                buf[i - 1].value.number == 1)))
        {
            remove_eo_idx(buf, new_len, i - 1);
            new_len--;
            remove_eo_idx(buf, new_len, i - 1);
            new_len--;
        }
        // 0+
        // -0 should be filtered, 0- should not
        if (((buf[i].type == NUMBER && buf[i].value.number == 0 &&
                (buf[i - 1].type == ADD || buf[i - 1].type == SUB)) ||
            (buf[i].type == ADD && buf[i - 1].type == NUMBER &&
                buf[i - 1].value.number == 0)) && i < new_len - 1 && (buf[i + 1].type == ADD || buf[i + 1].type == SUB))
        {
            remove_eo_idx(buf, new_len, i - 1);
            new_len--;
            remove_eo_idx(buf, new_len, i - 1);
            new_len--;
        }
        i++;
    }

    new_len = 0;
    while (buf[new_len].type != END_LEX)
    {
        expression[new_len] = buf[new_len];
        new_len++;
    }
    expression[new_len] = buf[new_len];
    new_len++;

    return new_len;
}
