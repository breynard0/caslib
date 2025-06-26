#ifndef EO_TYPES
#define EO_TYPES
enum EOType {
  NUMBER,
  LETTER,
  MULT,
  DIV,
  ADD,
  SUB,
  PI_VAL,
  ROOT,
  EXP,
  BLOCK_START,
  BLOCK_END,
  EQUAL,
  SINE,
  COSINE,
  TANGENT,
  ARCSINE,
  ARCCOSINE,
  ARCTANGENT,
  LOG,
  END_LEX
};

union EOVal {
  double number;
  struct {
    char letter;
    char subscript;
  } letter;
  struct {
    short count;
    int start;
  } block;
  char none;
};

struct EquationObject {
  enum EOType type;
  union EOVal value;
};
#endif
