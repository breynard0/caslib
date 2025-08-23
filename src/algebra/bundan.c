#include "cauchy.h"
#include "expansion.h"

int get_sign_changes(const struct EquationObject* expression, const int length)
{
    int changes = 0;
    short last_sign = 0;

    for (int i = 0; i < length; i++)
    {
        if (i == 0 || expression[i - 1].type == ADD ||
            expression[i - 1].type == SUB)
        {
            short cur_sign = 1;
            if (expression[i - 1].type == SUB)
            {
                cur_sign = -1;
            }
            if (expression[i].type == NUMBER && i != 0 &&
                ((expression[i].value.number < 0 && expression[i - 1].type == ADD) ||
                    (expression[i].value.number > 0 && expression[i - 1].type == SUB)))
            {
                cur_sign = -1;
            }
            if (i == 0 && expression[i].type == NUMBER &&
                expression[i].value.number < 0)
            {
                cur_sign = -1;
            }
            if (last_sign != 0 && cur_sign != last_sign)
            {
                changes++;
            }
            last_sign = cur_sign;
        }
    }

    return changes;
}

int sub_letters(struct EquationObject* buffer, const int length, const int max_len,
                const struct EquationObject* sub, const int sub_len)
{
    int new_len = length;
    int i = 0;
    while (i < new_len && new_len < max_len)
    {
        if (buffer[i].type == LETTER)
        {
            remove_eo_idx(buffer, new_len, i);
            new_len--;
            for (int j = 0; j < sub_len; j++)
            {
                new_len++;
                insert_eo_idx(buffer, new_len, i + j, sub[j]);
            }
            i += sub_len - 1;
        }
        i++;
    }
    return new_len;
}

int bundan_max_roots(const struct EquationObject* expression, const int length, const double min,
                     const double max)
{
    struct Letter target;
    target.letter = 'x';
    target.subscript = ' ';
    for (int i = 0; i < length; i++)
    {
        if (expression[i].type == LETTER)
        {
            target = expression[i].value.letter;
            break;
        }
    }

    struct EquationObject replace_array[5];
    replace_array[0].type = BLOCK_START;

    replace_array[1].type = LETTER;
    replace_array[1].value.letter = target;

    replace_array[2].type = ADD;

    replace_array[3].type = NUMBER;

    replace_array[4].type = BLOCK_END;

    struct EquationObject first[3 * length];
    int first_len = 0;
    for (int i = 0; i < length; i++)
    {
        first[first_len] = expression[i];
        first_len++;
    }
    replace_array[3].value.number = min;
    if (min != 0)
    {
        first_len = sub_letters(first, first_len, 3 * length, replace_array, 5);
        first_len = expand_polynomial(first, first_len);
    }
    const int first_sign_changes = get_sign_changes(first, first_len);

    struct EquationObject second[3 * length];
    int second_len = 0;
    for (int i = 0; i < length; i++)
    {
        second[second_len] = expression[i];
        second_len++;
    }
    replace_array[3].value.number = max;
    second_len = sub_letters(second, second_len, 3 * length, replace_array, 5);
    second_len = expand_polynomial(second, second_len);
    const int second_sign_changes = get_sign_changes(second, second_len);

    return first_sign_changes - second_sign_changes;
}