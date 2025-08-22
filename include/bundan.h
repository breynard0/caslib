#include "equation_objects.h"
int get_sign_changes(struct EquationObject* expression, int length);
int bundan_max_roots(struct EquationObject* expression, int length, double min,
                     double max);
int sub_letters(struct EquationObject* buffer, int length, int max_len,
                struct EquationObject* sub, int sub_len);