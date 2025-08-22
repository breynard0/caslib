#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"
#include "flags.h"
#include "gcf.h"

struct ReplaceObject
{
    struct Letter letter;
    short start_idx;
    short end_idx;
};

int rearrange_for_var(struct EquationObject* buffer, int length,
                      struct Letter target)
{
    // Grab copy of original, making sure target is present in the process
    Boolean target_present = FALSE;
    struct EquationObject original[length] = {};
    for (int i = 0; i < length; i++)
    {
        original[i] = buffer[i];
        if (buffer[i].type == LETTER &&
            buffer[i].value.letter.letter == target.letter &&
            buffer[i].value.letter.subscript == target.subscript)
        {
            target_present = TRUE;
        }
    }

    if (!target_present)
    {
        f_bad_equation = TRUE;
        return 0;
    }

    // Construct ReplaceObject array
    struct ReplaceObject r_objs[length / 2] = {};
    int new_len = length;
    int r_objs_len = 0;
    for (int i = 0; i < length; i++)
    {
        struct ReplaceObject temp;
        temp.letter.letter = '#';
        temp.letter.subscript = r_objs_len;

        switch (buffer[i].type)
        {
        // Single character values
        case PI_VAL:
        case NUMBER:
            if (i == 0 ||
                !(buffer[i - 1].type == SINE || buffer[i - 1].type == COSINE ||
                    buffer[i - 1].type == TANGENT || buffer[i - 1].type == ARCSINE ||
                    buffer[i - 1].type == ARCCOSINE ||
                    buffer[i - 1].type == ARCTANGENT || buffer[i - 1].type == TANGENT ||
                    buffer[i - 1].type == ARCSINE || buffer[i + 1].type == ROOT ||
                    buffer[i - 1].type == ROOT || buffer[i + 1].type == LOG ||
                    buffer[i - 1].type == LOG))
            {
                temp.start_idx = i;
                temp.end_idx = i;
                buffer[i].type = LETTER;
                buffer[i].value.letter = temp.letter;
                r_objs[r_objs_len] = temp;
                r_objs_len++;
            }
            break;
        case SINE:
        case COSINE:
        case TANGENT:
        case ARCSINE:
        case ARCCOSINE:
        case ARCTANGENT:
            temp.start_idx = i;
            temp.end_idx = i + 1;
            buffer[i].type = LETTER;
            buffer[i].value.letter = temp.letter;
            remove_eo_idx(buffer, new_len, i + 1);
            new_len--;
            r_objs[r_objs_len] = temp;
            r_objs_len++;
            break;
        // Double input functions
        case ROOT:
        case LOG:
        case EXP:
            if (buffer[i - 1].type != LETTER && buffer[i + 1].type != LETTER)
            {
                temp.start_idx = i - 1;
                temp.end_idx = i + 1;
                buffer[i - 1].type = LETTER;
                buffer[i].value.letter = temp.letter;
                remove_eo_idx(buffer, new_len, i);
                new_len--;
                remove_eo_idx(buffer, new_len, i);
                new_len--;
                r_objs[r_objs_len] = temp;
                r_objs_len++;
            }
            break;
        default:
            break;
        }
    }

    struct EquationObject lhs[length] = {};
    int lhs_len = 0;
    struct EquationObject rhs[2 * length] = {};
    int rhs_len = 0;

    Boolean equal_found = FALSE;

    int i = 0;
    {
        int start = 0;
        Boolean target_found = FALSE;
        while (i < length)
        {
            enum EOType cur_type = buffer[i].type;
            if (cur_type == LETTER &&
                buffer[i].value.letter.letter == target.letter &&
                buffer[i].value.letter.subscript == target.subscript)
            {
                target_found = TRUE;
            }

            if (cur_type == ADD || cur_type == SUB || cur_type == EQUAL ||
                cur_type == END_LEX || i == length - 1)
            {
                // Make pointers to make the code cleaner
                struct EquationObject* buf_ptr;
                int* len_ptr;
                if (target_found)
                {
                    buf_ptr = lhs;
                    len_ptr = &lhs_len;
                }
                else
                {
                    buf_ptr = rhs;
                    len_ptr = &rhs_len;
                }

                // Get sign of the term
                int sign_idx = start - 1;
                Boolean is_negative = FALSE;
                if (sign_idx >= 0 && buffer[sign_idx].type == SUB)
                {
                }
                // Flip the sign if target_found and equal_found have the same value
                if (target_found == equal_found)
                {
                    is_negative = !is_negative;
                }
                // Push the sign, unless it's both positive and start is 0
                if (is_negative)
                {
                    buf_ptr[*len_ptr].type = SUB;
                    (*len_ptr)++;
                }
                else
                {
                    if (start != 0)
                    {
                        buf_ptr[*len_ptr].type = ADD;
                        (*len_ptr)++;
                    }
                }

                for (int j = start; j < i; j++)
                {
                    buf_ptr[*len_ptr] = buffer[j];
                    (*len_ptr)++;
                }
                start = i + 1;
                target_found = FALSE;
            }

            // This should come afterwards because i refers the the previous term
            if (cur_type == EQUAL)
            {
                equal_found = TRUE;
            }

            i++;
        }
    }

    // Divide all terms by GCD of left hand side

    // First, get the GCD
    struct EquationObject gcd_buf[length] = {};
    int gcd_len = 0;

    // Load first term into GCD
    // Move the index to the second term
    i = 0;
    while (i < lhs_len)
    {
        if (lhs[i].type == ADD || lhs[i].type == SUB)
        {
            i++;
            break;
        }
        i++;
    }
    // Populate GCD array while removing target from the GCD, reusing code I've
    // already written
    gcd_len = term_gcf(lhs, i - 1, lhs, i - 1, gcd_buf, length, target);

    // For next terms, get GCD between them and this
    Boolean found = FALSE;
    int start = i;
    struct EquationObject tmp_buf[length] = {};
    while (i <= lhs_len)
    {
        if (lhs[i].type == ADD || lhs[i].type == SUB || i == lhs_len)
        {
            found = TRUE;
            for (int j = 0; j < gcd_len; j++)
            {
                tmp_buf[j] = gcd_buf[j];
            }
            gcd_len = term_gcf(tmp_buf, gcd_len, &lhs[start], i - start, gcd_buf,
                               length, target);
            start = i + 1;
        }
        i++;
    }

    // Divide both sides by the GCD
    lhs_len = div_terms(lhs, lhs_len, gcd_buf, gcd_len) - 1;
    rhs_len = div_terms(rhs, rhs_len, gcd_buf, gcd_len) - 1;

    // Get degree of variables
    Boolean same = TRUE;
    double cur_degree = 0.0 / 0.0;
    for (int i = 0; i < lhs_len; i++)
    {
        if (lhs[i].type == LETTER && lhs[i].value.letter.letter == target.letter)
        {
            double tent_degree = 1;
            if (lhs[i].value.letter.subscript == target.subscript &&
                i < lhs_len - 2 && lhs[i + 1].type == EXP)
            {
                tent_degree = lhs[i + 2].value.number;
            }
            if (cur_degree != cur_degree)
            {
                cur_degree = tent_degree;
            }
            if (tent_degree != cur_degree)
            {
                same = FALSE;
            }
        }
    }

    // Expand rhs now because it may end up with a divisor
    rhs_len = expand_polynomial(rhs, rhs_len) - 1;

    // If all degrees are the same, factor out x then divide rhs by that quotient
    // Afterwards, set lhs to the variable
    if (same)
    {
        struct EquationObject div_obj[4] = {};
        div_obj[0].type = LETTER;
        div_obj[0].value.letter = target;
        div_obj[1].type = EXP;
        div_obj[2].type = NUMBER;
        div_obj[2].value.number = cur_degree;
        lhs_len = div_terms(lhs, lhs_len, div_obj, 3) - 1;

        struct EquationObject bs_obj = {};
        bs_obj.type = BLOCK_START;
        rhs_len++;
        insert_eo_idx(rhs, rhs_len, 0, bs_obj);
        rhs[rhs_len].type = BLOCK_END;
        rhs_len++;
        rhs[rhs_len].type = DIV;
        rhs_len++;
        rhs[rhs_len].type = BLOCK_START;
        rhs_len++;
        for (int j = 0; j < lhs_len; j++)
        {
            rhs[rhs_len] = lhs[j];
            rhs_len++;
        }
        rhs[rhs_len].type = BLOCK_END;
        rhs_len++;
        rhs[rhs_len].type = END_LEX;
        rhs_len++;

        for (int j = 0; j < 4; j++)
        {
            lhs[j] = div_obj[j];
        }
        lhs[3].type = END_LEX;
        lhs_len = 4;
    }

    // Expand lhs
    lhs_len = expand_polynomial(lhs, lhs_len) - 1;

    // Substitute in values
    int max = lhs_len;
    if (rhs_len > lhs_len)
    {
        max = rhs_len;
    }
    for (i = 0; i < max; i++)
    {
        for (int j = 0; j < r_objs_len; j++)
        {
            if (i < lhs_len && lhs[i].type == LETTER &&
                lhs[i].value.letter.letter == r_objs[j].letter.letter &&
                lhs[i].value.letter.subscript == r_objs[j].letter.subscript)
            {
                remove_eo_idx(lhs, lhs_len, i);
                lhs_len--;
                for (int k = 0; k <= r_objs[j].end_idx - r_objs[j].start_idx; k++)
                {
                    lhs_len++;
                    insert_eo_idx(lhs, lhs_len, i + k, original[r_objs[j].start_idx + k]);
                }
            }
            if (i < rhs_len && rhs[i].type == LETTER &&
                rhs[i].value.letter.letter == r_objs[j].letter.letter &&
                rhs[i].value.letter.subscript == r_objs[j].letter.subscript)
            {
                remove_eo_idx(rhs, rhs_len, i);
                rhs_len--;
                for (int k = 0; k <= r_objs[j].end_idx - r_objs[j].start_idx; k++)
                {
                    rhs_len++;
                    insert_eo_idx(rhs, rhs_len, i + k, original[r_objs[j].start_idx + k]);
                }
            }
        }
    }

    // Push to buffer
    int out_len = 0;
    i = 0;

    // GCD debugging
    // while (i < gcd_len) {
    //   buffer[out_len] = gcd_buf[i];
    //   out_len++;
    //   i++;
    // }

    while (i < lhs_len)
    {
        buffer[out_len] = lhs[i];
        out_len++;
        i++;
    }
    buffer[out_len].type = EQUAL;
    out_len++;
    i = 0;
    while (i < rhs_len)
    {
        buffer[out_len] = rhs[i];
        out_len++;
        i++;
    }
    buffer[out_len].type = END_LEX;
    out_len++;
    return out_len;
}