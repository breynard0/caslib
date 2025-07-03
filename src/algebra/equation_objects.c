#include "equation_objects.h"

void remove_eo_idx(struct EquationObject *list, int length, int idx) {
  if (idx == length - 1) {
    list[idx].type = END_LEX;
  }

  for (int i = idx; i < length - 1; i++) {
    list[i] = list[i + 1];
  }
}

// Make sure list has enough space, pre-increment
void insert_eo_idx(struct EquationObject *list, int length, int idx, struct EquationObject object) {
  for (int i = length - 2; i >= idx; i--) {
    list[i + 1] = list[i];
  }
  list[idx] = object;
}
