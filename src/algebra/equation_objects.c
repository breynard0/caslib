#include "equation_objects.h"
#include "expansion.h"

void remove_eo_idx(struct EquationObject *list, int length, int idx) {
  if (idx == length - 1) {
    list[idx].type = END_LEX;
  }

  for (int i = idx; i < length - 1; i++) {
    list[i] = list[i + 1];
  }
}

// Make sure list has enough space, pre-increment
void insert_eo_idx(struct EquationObject *list, int length, int idx,
                   struct EquationObject object) {
  for (int i = length - 2; i >= idx; i--) {
    list[i + 1] = list[i];
  }
  list[idx] = object;
}

// Yes, this is a wrapper. Yes, I'll fix it if it's a performance issue.
int div_terms(struct EquationObject *term0, int t0_len,
              struct EquationObject *term1, int t1_len) {
  struct EquationObject tmp_buf[2 * (t0_len + t1_len)];
  int tmp_len = 0;
  for (int i = 0; i < t0_len; i++) {
    tmp_buf[tmp_len] = term0[i];
    tmp_len++;
  }
  tmp_buf[tmp_len].type = DIV;
  tmp_len++;
  tmp_buf[tmp_len].type = BLOCK_START;
  tmp_len++;
  for (int i = 0; i < t1_len; i++) {
    tmp_buf[tmp_len] = term1[i];
    tmp_len++;
  }
  tmp_buf[tmp_len].type = BLOCK_END;
  tmp_len++;
  
  int new_len = expand_polynomial(tmp_buf, tmp_len);
  
  for (int i = 0; i < new_len; i++) {
    term0[i] = tmp_buf[i];
  }
  
  return new_len;
}
