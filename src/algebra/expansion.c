#include "collection.h"
#include "cull.h"
#include "dutils.h"
#include "evaluate.h"
#include "flags.h"
#include "parse.h"
#include "solve_consts.h"
#include "utils.h"

// Make sure buffer is big enough! This function also assumes no block
// denominators.
// Length should also be one less than the length of the buffer
int expand_polynomial(struct EquationObject* buffer, int length)
{
    // If a single thing, do nothing
    if (length <= 2)
    {
        length = 2;
        return length;
    }

    // Remove empty parentheses
    int par_idx = 1;
    while (par_idx < length)
    {
        if (buffer[par_idx].type == BLOCK_END &&
            buffer[par_idx - 1].type == BLOCK_START)
        {
            remove_eo_idx(buffer, length, par_idx);
            length--;
            remove_eo_idx(buffer, length, par_idx - 1);
            length--;
            par_idx -= 2;
        }
        par_idx++;
    }

    int new_len = 0;
    while (buffer[new_len].type != END_LEX)
    {
        new_len++;
    }
    new_len++;

    // Get number of juxtaposed elements to know how long to make buffer
    int extra_count = 0;
    for (int i = 1; i < length; i++)
    {
        if (buffer[i].type == END_LEX)
        {
            break;
        }
        if (is_juxtaposed(buffer[i], buffer[i - 1]))
        {
            extra_count++;
        }
        if (is_negative(buffer[i], buffer[i - 1]))
        {
            extra_count += 3;
        }
        if ((buffer[i].type == NUMBER || buffer[i].type == LETTER ||
                buffer[i].type == BLOCK_START) &&
            buffer[i - 1].type == BLOCK_END)
        {
            int block_count = 1;
            int j = i - 2;
            int extra = 0;
            while (block_count != 0)
            {
                if (buffer[j].type == BLOCK_END)
                {
                    block_count++;
                }
                if (buffer[j].type == BLOCK_START)
                {
                    block_count--;
                }
                j--;
                extra++;
            }
            extra++;
            extra_count += extra * (new_len / extra);
        }
        if (buffer[i].type == EXP && buffer[i - 1].type == BLOCK_END)
        {
            extra_count += i * (int)buffer[i + 1].value.number;
        }
    }

    int orig_expr_len = 2 * (new_len + 2 * extra_count) + 1;

    struct EquationObject expression[orig_expr_len] = {};

    expand_juxtopposed(buffer, new_len, expression, orig_expr_len, 0, 0);

    new_len = 0;
    while (expression[new_len].type != END_LEX)
    {
        new_len++;
    }
    new_len++;

    new_len = cull_the_useless(expression, new_len);

    new_len = evaluate_consts(expression, new_len);

    new_len = cull_the_useless(expression, new_len);

    struct EquationObject mult_obj;
    mult_obj.type = MULT;

    // Expand term powers
    int i = 1;
    while (i < new_len)
    {
        if (expression[i].type == EXP && expression[i - 1].type == BLOCK_END)
        {
            double exp = expression[i + 1].value.number;

            // Grab term
            int start = i - 2;
            while (expression[start].type != BLOCK_START)
            {
                start--;
            }

            int term_len = i - start;
            struct EquationObject term[term_len] = {};
            for (int j = 0; j < term_len; j++)
            {
                term[j] = expression[start + j];
            }

            // Remove exponent
            remove_eo_idx(expression, new_len, i);
            new_len--;
            remove_eo_idx(expression, new_len, i);
            new_len--;

            // Copy term
            for (int j = 0; j < exp - 1; j++)
            {
                new_len++;
                insert_eo_idx(expression, new_len, i, mult_obj);
                i++;
                for (int k = 0; k < term_len; k++)
                {
                    new_len++;
                    insert_eo_idx(expression, new_len, i, term[k]);
                    i++;
                }
            }
            i = 0;
        }
        i++;
    }

    // Rewrite division as multiplication of negative exponents
    struct EquationObject exp_obj;
    exp_obj.type = EXP;
    struct EquationObject minus_1_obj;
    minus_1_obj.type = NUMBER;
    minus_1_obj.value.number = -1;
    i = 0;
    Boolean found = FALSE;
    while (i < new_len)
    {
        if (expression[i].type == DIV)
        {
            found = TRUE;
            expression[i].type = MULT;
            if (expression[i + 1].type == NUMBER ||
                expression[i + 1].type == LETTER)
            {
                // Divison by zero
                if (expression[i + 1].type == NUMBER && double_abs(expression[i + 1].value.number) < THRESHOLD)
                {
                    f_undefined = TRUE;
                    return 0;
                }

                new_len++;
                insert_eo_idx(expression, new_len, i + 2, minus_1_obj);
                new_len++;
                insert_eo_idx(expression, new_len, i + 2, exp_obj);
            }
            if (expression[i + 1].type == BLOCK_START)
            {
                while (expression[i].type != BLOCK_END)
                {
                    i++;
                }
                i--;
                int start = i;
                while (expression[start].type != BLOCK_START)
                {
                    start--;
                }
                start++;
                struct EquationObject tmp_buf[4 * (i - start + 1)];
                int tmp_len = 0;
                for (int k = 0; k < i - start + 1; k++)
                {
                    tmp_buf[tmp_len] = expression[start + k];
                    tmp_len++;
                }
                tmp_buf[tmp_len].type = END_LEX;
                tmp_len++;

                tmp_len = expand_polynomial(tmp_buf, tmp_len);

                int j = 0;
                while (j < tmp_len)
                {
                    if (tmp_buf[j].type == MULT || tmp_buf[j].type == END_LEX ||
                        j >= tmp_len - 1)
                    {
                        if (j >= 2 && tmp_buf[j - 1].type == NUMBER &&
                            tmp_buf[j - 2].type == EXP)
                        {
                            tmp_buf[j - 1].value.number *= -1;
                        }
                        else
                        {
                            tmp_len++;
                            insert_eo_idx(tmp_buf, tmp_len, j, minus_1_obj);
                            tmp_len++;
                            insert_eo_idx(tmp_buf, tmp_len, j, exp_obj);
                            j += 2;
                        }
                    }
                    // Code doesn't know how to divide by polynomials yet
                    // Rearrangment doesn't need it and root-finding can ignore it
                    if (tmp_buf[j].type == ADD || tmp_buf[j].type == SUB)
                    {
                        f_unsupported = TRUE;
                        return 0;
                    }
                    j++;
                }
                for (int k = start; k <= i; k++)
                {
                    remove_eo_idx(expression, new_len, start);
                    new_len--;
                }
                for (int k = 0; k < tmp_len - 1; k++)
                {
                    new_len++;
                    insert_eo_idx(expression, new_len, start + k, tmp_buf[k]);
                }
            }
        }

        i++;
    }

    if (found)
    {
        new_len = evaluate_consts(expression, new_len);
    }

    // Rearrange to make solvable
    i = 1;
    while (i < new_len)
    {
        struct EquationObject self = expression[i];
        struct EquationObject last = expression[i - 1];
        if (last.type == BLOCK_END && self.type == MULT)
        {
            int start = i - 2;
            int bracket_count = 1;
            while (bracket_count != 0)
            {
                if (expression[start].type == BLOCK_END)
                {
                    bracket_count++;
                }
                if (expression[start].type == BLOCK_START)
                {
                    bracket_count--;
                }
                start--;
            }
            start++;
            int factor_count = i - start;
            struct EquationObject factor[factor_count] = {};
            for (int j = 0; j < factor_count; j++)
            {
                factor[j] = expression[start];
                remove_eo_idx(expression, new_len, start);
                new_len--;
                i--;
            }

            int dest_start = i + 1;
            int end = -1;

            if (expression[dest_start].type == LETTER ||
                expression[dest_start].type == NUMBER)
            {
                while (expression[dest_start].type != ADD &&
                    expression[dest_start].type != SUB && dest_start < new_len - 2)
                {
                    dest_start++;
                }
                for (int j = 0; j < factor_count; j++)
                {
                    new_len++;
                    insert_eo_idx(expression, new_len, dest_start + j + 1, factor[j]);
                }
                new_len++;
                insert_eo_idx(expression, new_len, dest_start + 1, mult_obj);
            }

            if (expression[dest_start].type == BLOCK_START)
            {
                while (expression[dest_start].type == BLOCK_START)
                {
                    dest_start++;
                    start++;
                }
                dest_start--;
                start--;
                int idx = dest_start + 1;
                // Avoid placing inside bracket terms
                int block_count = 0;
                Boolean running = TRUE;
                while (running)
                {
                    if (expression[idx].type == BLOCK_START)
                    {
                        block_count++;
                    }
                    const enum EOType last1 = expression[idx].type;
                    if (block_count == 0 &&
                        (expression[idx].type == ADD || expression[idx].type == SUB ||
                            expression[idx].type == BLOCK_END))
                    {
                        new_len++;
                        insert_eo_idx(expression, new_len, idx, mult_obj);
                        for (int j = 0; j < factor_count; j++)
                        {
                            idx++;
                            new_len++;
                            insert_eo_idx(expression, new_len, idx, factor[j]);
                        }
                        idx++;
                    }
                    if (last1 == BLOCK_END)
                    {
                        if (block_count == 0)
                        {
                            running = FALSE;
                        }
                        else
                        {
                            block_count--;
                        }
                    }
                    idx++;
                }
                end = idx - 2;
            }

            // Remove leading multiplication sign
            remove_eo_idx(expression, new_len, i);
            new_len--;

            // Inline new thing, reusing buffer
            if (end > 0)
            {
                int idx = start;
                int j = 0;
                while (idx <= end)
                {
                    buffer[j] = expression[idx];

                    idx++;
                    j++;
                }
                buffer[j].type = END_LEX;
                j++;
                int out_count = expand_polynomial(buffer, j) - 1;
                for (int k = 0; k < end - start; k++)
                {
                    remove_eo_idx(expression, new_len, start);
                    new_len--;
                }

                new_len++;
                struct EquationObject block_start;
                block_start.type = BLOCK_START;
                insert_eo_idx(expression, new_len, start, block_start);

                new_len += out_count;
                for (int k = 0; k < out_count; k++)
                {
                    insert_eo_idx(expression, new_len, start + 1 + k, buffer[k]);
                }
            }

            i = -1;
        }
        i++;
    }

    new_len = cull_the_useless(expression, new_len);

    // Replace subtracting with negative addition
    // It helps make distribution work better
    i = 1;
    while (i < new_len)
    {
        if (expression[i - 1].type == SUB)
        {
            expression[i - 1].type = ADD;

            struct EquationObject obj;
            obj.type = NUMBER;
            obj.value.number = -1.0;

            new_len++;
            insert_eo_idx(expression, new_len, i, mult_obj);
            new_len++;
            insert_eo_idx(expression, new_len, i, obj);
        }
        i++;
    }

    // Distributive property
    i = 0;
    while (i < new_len)
    {
        if (expression[i].type == BLOCK_END)
        {
            int start = i;
            while (expression[start].type != BLOCK_START)
            {
                start--;
            }

            if (start != 0)
            {
                if (expression[start - 1].type == MULT ||
                    expression[start - 1].type == DIV)
                {
                    enum EOType op = expression[start - 1].type;
                    int prestart = start - 2;
                    while (expression[prestart].type != ADD &&
                        expression[prestart].type != SUB &&
                        expression[prestart].type != BLOCK_START)
                    {
                        if (prestart > 0)
                        {
                            prestart--;
                        }
                        else
                        {
                            break;
                        }
                    }
                    if (expression[prestart].type == ADD ||
                        expression[prestart].type == SUB ||
                        expression[prestart].type == BLOCK_START)
                    {
                        prestart++;
                    }
                    int factor_len = start - prestart;
                    struct EquationObject factor[factor_len];
                    for (int n = 0; n < factor_len; n++)
                    {
                        factor[n] = expression[n + prestart];
                    }

                    // Removes starting block
                    remove_eo_idx(expression, new_len, start);
                    new_len--;
                    int j = start;
                    while (j < new_len)
                    {
                        if (expression[j].type == BLOCK_END)
                        {
                            remove_eo_idx(expression, new_len, j);
                            new_len--;
                            break;
                        }

                        if (expression[j].type == ADD || expression[j].type == SUB)
                        {
                            j++;
                            for (int k = 0; k < factor_len; k++)
                            {
                                new_len++;
                                insert_eo_idx(expression, new_len, j + k, factor[k]);
                            }
                        }

                        if (expression[j].type == MULT && op == DIV)
                        {
                            expression[j].type = DIV;
                        }

                        j++;
                    }
                }
                else if (expression[start - 1].type == BLOCK_START)
                {
                    // Double parentheses
                    remove_eo_idx(expression, new_len, i);
                    new_len--;
                    remove_eo_idx(expression, new_len, start);
                    new_len--;
                    i = 0;
                }
            }
        }

        i++;
    }

    new_len = cull_the_useless(expression, new_len);

    // Add end_lex if it is missing
    if (expression[new_len - 1].type != END_LEX)
    {
        expression[new_len].type = END_LEX;
    }

    new_len = 0;
    while (expression[new_len].type != END_LEX)
    {
        new_len++;
    }
    new_len++;

    // Collect terms
    new_len = collect_reorder_polynomial(expression, new_len);

    for (int m = 0; m < new_len; m++)
    {
        buffer[m] = expression[m];
    }
    return new_len;
}