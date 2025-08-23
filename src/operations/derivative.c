#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"

int power_rule_derivative_univariate(struct EquationObject* function,
                                     int length)
{
    // struct EquationObject function[length] = {};
    // for (int i = 0; i < length; i++) {
    //   function[i] = fudnction[i];
    // }

    // If highest degree is 1 or lower, return 1
    const int degree = get_polynomial_degree(function, length);
    if (degree <= 1)
    {
        function[0].type = NUMBER;
        function[0].value.number = 1.0;
        function[1].type = END_LEX;
        return 2;
    }

    int new_len = length;

    int i = 0;
    while (i < new_len)
    {
        if (function[i].type == ADD || function[i].type == SUB ||
            function[i].type == END_LEX || i == new_len - 1)
        {
            int start = i - 1;
            while (function[start].type != ADD && function[start].type != SUB &&
                start > 0)
            {
                start--;
            }
            if (function[start].type == ADD || function[start].type == SUB)
            {
                start++;
            }

            // Get n and reduce exponent
            double n = 1.0;
            Boolean exp_found = FALSE;
            for (int j = 0; j < i - start; j++)
            {
                if (function[start + j].type == EXP)
                {
                    exp_found = TRUE;
                    n = function[start + j + 1].value.number;
                    function[start + j + 1].value.number -= 1.0;
                    break;
                }
            }
            Boolean letter_found = FALSE;
            for (int j = 0; j < i - start; j++)
            {
                if (function[start + j].type == LETTER)
                {
                    letter_found = TRUE;
                }
            }
            n *= letter_found;

            if (n == 0.0)
            {
                const int del = start - 1;
                for (int j = 0; j <= (i - start) + 1; j++)
                {
                    remove_eo_idx(function, new_len, del);
                    new_len--;
                    i--;
                }
            }

            if (n == 1.0 || (!exp_found && letter_found))
            {
                int letter_idx = start;
                while (function[letter_idx].type != LETTER)
                {
                    letter_idx++;
                }
                for (int j = 0; j < i - letter_idx; j++)
                {
                    remove_eo_idx(function, new_len, letter_idx);
                    new_len--;
                    i--;
                }
                if (letter_idx > 0 && function[letter_idx - 1].type == MULT)
                {
                    remove_eo_idx(function, new_len, letter_idx - 1);
                    new_len--;
                    i--;
                }
                if (start >= new_len || function[start].type != NUMBER)
                {
                    struct EquationObject one_obj;
                    one_obj.type = NUMBER;
                    one_obj.value.number = 1.0;

                    insert_eo_idx(function, new_len, start, one_obj);
                    new_len++;
                    i++;
                }
            }

            if (n > 1.0)
            {
                if (function[start].type == NUMBER)
                {
                    function[start].value.number *= n;
                }
                else
                {
                    struct EquationObject num_obj;
                    num_obj.type = NUMBER;
                    num_obj.value.number = n;
                    new_len++;
                    insert_eo_idx(function, new_len, start, num_obj);
                    i++;
                    struct EquationObject mult_obj;
                    mult_obj.type = MULT;
                    new_len++;
                    insert_eo_idx(function, new_len, start + 1, mult_obj);
                    i++;
                }
            }
        }
        i++;
    }

    new_len = expand_polynomial(function, new_len);

    return new_len;
}