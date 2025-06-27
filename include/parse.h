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
        char letter[2];
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

#endif

// Parses one side of equation, will stop at either an equals sign or an end
// character Returns length of buffer
int parse_expr(struct EquationObject *input, int length,
          struct ParseObject *buffer, int max_length);
