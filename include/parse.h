#include "enums.h"
#include "equation_objects.h"

#ifndef PO_OBJ
#define PO_OBJ
enum POType { MAKE_TEMP, COMBINE };

enum OpType {
  OP_ADD,
  OP_SUB,
  OP_MULT,
  OP_DIV,
  OP_POW,
  OP_ROOT,
  OP_SIN,
  OP_COS,
  OP_TAN,
  OP_ASIN,
  OP_ACOS,
  OP_ATAN,
  OP_LOG
};

enum POArgType { TEMP_VAR, NUM, COEFF_VAR };

struct POArg {
  enum POArgType type;
  union {
    double num;
    int var_idx;
    struct {
      double co_eff;
      struct Letter letter;
    } coeff_var;
  } val;
};

struct POValue {
  int store_idx;
  short num_args;
  struct POArg args[];
};

struct ParseObject {
  enum POType type;
  struct POValue value;
};

struct InputVar {
  struct Letter letter;
  double value;
};
#endif

Boolean is_juxtaposed(struct EquationObject self, struct EquationObject last);
Boolean is_negative(struct EquationObject self, struct EquationObject last);
int expand_juxtopposed(struct EquationObject *input, int length,
                       struct EquationObject *out_buffer, int buffer_len, struct InputVar* buffer,
                       int num_args);
