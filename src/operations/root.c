#include "bisection.h"
#include "dutils.h"
#include "expansion.h"
#include "flags.h"
#include "log.h"
#include "pow.h"
#include "solve_consts.h"
#include "utils.h"
#include "yun.h"

double nth_root(const unsigned int n, double d)
{
    // Generalization of Heron/Newton's method
    // Invalid check
    if (n % 2 == 0 && d < 0)
    {
        return NAN;
    }

    // Use double exponent to use square root of close-ish power of 2 as first
    // estimate
    const double_cast cast = (double_cast)d;
    double estimate = pow_ll(2, (cast.parts.exponent - 1023) / 2);

    const double fn = n;

    double small_mult = 1.0;
    if (d < 0.1 || d > 10000)
    {
        small_mult = 100.0;
    }

    // I've arbitrarily picked the number of iterations until it seems precise
    // enough
    const int ITERATIONS = 4;
    for (int i = 0; i < ITERATIONS * n * small_mult; i++)
    {
        estimate =
            1.0 / fn * ((fn - 1.0) * estimate + d / pow_di(estimate, n - 1));
    }

    if (n % 2 == 0)
    {
        return double_abs(estimate);
    }
    return estimate;
}

double square_root(const double d) { return nth_root(2, d); }
double cube_root(const double d) { return nth_root(3, d); }

int real_roots(struct EquationObject* expression, const int length, double* roots)
{
    // Fail if there are coefficients more than 3 orders of magnitude apart, or greater than 9
    const double magnitude_threshold = 3.0;
    double lowest_coeff = 0.0 / 0.0;
    double highest_coeff = 0.0 / 0.0;
    for (int i = 0; i < length; i++)
    {
        if (expression[i].type == NUMBER && (i == 0 || expression[i - 1].type != EXP))
        {
            const double val = expression[i].value.number;

            // NaN checks
            if (lowest_coeff != lowest_coeff)
            {
                // ReSharper disable once CppDFAUnreachableCode
                lowest_coeff = val;
            }
            if (highest_coeff != highest_coeff)
            {
                // ReSharper disable once CppDFAUnreachableCode
                highest_coeff = val;
            }

            // Replace if bigger or smaller
            if (val < lowest_coeff)
            {
                lowest_coeff = val;
            }
            if (val > highest_coeff)
            {
                highest_coeff = val;
            }
        }
    }
    if (double_abs(log_n(lowest_coeff, 10) - log_n(highest_coeff, 10)) > magnitude_threshold || log_n(highest_coeff, 10) > 9)
    {
        f_unsupported = TRUE;
        return 0;
    }


    struct EquationObject original[length] = {};
    for (int i = 0; i < length; i++)
    {
        original[i] = expression[i];
    }
    int new_len = yun_decompose(expression, length);
    new_len = expand_polynomial(expression, new_len);
    struct RootRange delimiters[new_len] = {};
    const int out_delim_len =
        get_isolation_delimiter_positions(expression, new_len, delimiters);
    int num_roots = 0;
    for (int i = 0; i < out_delim_len; i++)
    {
        double root = get_root_bisection(delimiters[i], expression, new_len);
        // Round to nearest threshold unit
        root = dround(root / THRESHOLD) * THRESHOLD;
        struct InputVar var;
        int j = 0;
        while (expression[j].type != LETTER)
        {
            j++;
        }
        var.letter = expression[j].value.letter;
        var.value = root;
        // Verify that Yun's theorem hasn't made up roots. It can do that sometimes.
        const double y = solve_const_expr(original, length, &var, 1);
        if (double_abs(y) <= THRESHOLD * pow_di(10, 1 + log_n(highest_coeff, 10)))
        {
            roots[num_roots] = root;
            num_roots++;
        }
    }
    // This method is unable to detect roots of zero, so checking that here
    struct InputVar var;
    int i = 0;
    while (expression[i].type != LETTER)
    {
        i++;
    }
    var.letter = expression[i].value.letter;
    var.value = 0.0;
    const double y = solve_const_expr(original, length, &var, 1);
    if (double_abs(y) <= THRESHOLD * square_root(1.0 / THRESHOLD))
    {
        roots[num_roots] = 0.0;
        num_roots++;
    }
    return num_roots;
}
