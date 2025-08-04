#include "enums.h"
#include "equation_objects.h"
#include "expansion.h"

void polynomial_division(struct EquationObject *dividend, int dividend_len,
                         struct EquationObject *divisor, int divisor_len) {
  // Case of 2 constants
  if (dividend_len <= 2 && dividend[0].type == NUMBER && divisor_len <= 2 &&
      divisor[0].type == NUMBER) {
    dividend[0].value.number /= divisor[0].value.number;
    return;
  }

  int temp_max_len = dividend_len;
  if (divisor_len > dividend_len) {
    temp_max_len = divisor_len;
  }

  struct EquationObject quotient[4 * temp_max_len] = {};
  int quotient_len = 0;

  int divisor_highest_len = 0;
  while (divisor[divisor_highest_len].type != ADD &&
         divisor[divisor_highest_len].type != SUB &&
         divisor_highest_len < divisor_len) {
    divisor_highest_len++;
  }

  double divisor_degree = 1;
  Boolean letter_found = FALSE;
  for (int i = 0; i < divisor_highest_len; i++) {
    if (divisor[i].type == EXP) {
      divisor_degree = divisor[i + 1].value.number;
      break;
    } else if (divisor[i].type == LETTER) {
      letter_found = TRUE;
    }
  }
  if (!letter_found) {
    divisor_degree = 0;
  }

  struct EquationObject temp[4 * temp_max_len] = {};
  int temp_len = dividend_len;
  for (int i = 0; i < dividend_len; i++) {
    temp[i] = dividend[i];
  }

  double temp_degree = divisor_degree + 1;

  while (temp_degree >= divisor_degree) {
    if (temp[0].type == NUMBER && temp[0].value.number == 0) {
      break;
    }

    if (temp[temp_len - 1].type == END_LEX) {
      temp_len--;
    }

    struct EquationObject div_term[4 * temp_max_len] = {};
    int div_term_len = 0;

    if (quotient[quotient_len - 1].type == END_LEX) {
      quotient_len--;
    }

    if (quotient_len != 0) {
      quotient[quotient_len].type = ADD;
      quotient_len++;
    }

    quotient[quotient_len].type = BLOCK_START;
    quotient_len++;
    div_term[div_term_len].type = BLOCK_START;
    div_term_len++;

    int i = 0;
    while (temp[i].type != ADD && temp[i].type != SUB && i < temp_len) {
      quotient[quotient_len] = temp[i];
      quotient_len++;

      div_term[div_term_len] = temp[i];
      div_term_len++;

      i++;
    }

    quotient[quotient_len].type = BLOCK_END;
    quotient_len++;
    div_term[div_term_len].type = BLOCK_END;
    div_term_len++;

    quotient[quotient_len].type = DIV;
    quotient_len++;
    div_term[div_term_len].type = DIV;
    div_term_len++;

    quotient[quotient_len].type = BLOCK_START;
    quotient_len++;
    div_term[div_term_len].type = BLOCK_START;
    div_term_len++;

    i = 0;
    while (i < divisor_highest_len) {
      if (divisor[i].type == END_LEX) {
        break;
      }

      quotient[quotient_len] = divisor[i];
      quotient_len++;
      div_term[div_term_len] = divisor[i];
      div_term_len++;
      i++;
    }
    quotient[quotient_len].type = BLOCK_END;
    quotient_len++;
    div_term[div_term_len].type = BLOCK_END;
    div_term_len++;

    quotient[quotient_len].type = END_LEX;
    quotient_len++;
    div_term[div_term_len].type = END_LEX;
    div_term_len++;

    quotient_len = expand_polynomial(quotient, quotient_len);
    div_term_len = expand_polynomial(div_term, div_term_len);

    // Update temp
    struct EquationObject bs_obj;
    bs_obj.type = BLOCK_START;
    struct EquationObject be_obj;
    be_obj.type = BLOCK_END;
    temp_len++;
    insert_eo_idx(temp, temp_len, 0, bs_obj);
    if (temp[temp_len - 1].type == END_LEX) {
      temp[temp_len - 1].type = BLOCK_END;
    } else {
      temp_len++;
      insert_eo_idx(temp, temp_len, temp_len - 1, be_obj);
    }

    temp[temp_len].type = SUB;
    temp_len++;
    temp[temp_len].type = BLOCK_START;
    temp_len++;
    i = 0;
    while (i < div_term_len - 1) {
      temp[temp_len] = div_term[i];
      temp_len++;
      i++;
    }
    temp[temp_len].type = BLOCK_END;
    temp_len++;

    temp[temp_len].type = MULT;
    temp_len++;

    temp[temp_len].type = BLOCK_START;
    temp_len++;
    i = 0;
    while (i < divisor_len && divisor[i].type != END_LEX) {
      temp[temp_len] = divisor[i];
      temp_len++;
      i++;
    }
    temp[temp_len].type = BLOCK_END;
    temp_len++;
    temp[temp_len].type = END_LEX;
    temp_len++;
    // #include "debug.h"
    // for (int j = 0; j < temp_len; j++) {
    //   print_eo_flat(temp[j]);
    // }
    temp_len = expand_polynomial(temp, temp_len);

    temp_degree = 1;
    i = 0;
    Boolean letter_found = FALSE;
    while (i < temp_len) {
      if (temp[i].type == EXP) {
        temp_degree = temp[i + 1].value.number;
        break;
      } else if (temp[i].type == LETTER) {
        letter_found = TRUE;
      }

      i++;
    }
    if (!letter_found) {
      temp_degree = 0;
    }
  }

  // Set arrays
  for (int i = 0; i < quotient_len; i++) {
    dividend[i] = quotient[i];
  }
  for (int i = 0; i < temp_len; i++) {
    divisor[i] = temp[i];
  }
}
