#ifndef EO_TYPES
#define EO_TYPES

enum EOType
{
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
    END_LEX,
    // None must remain last
    NONE
};

struct Letter
{
    char letter;
    char subscript;
};

union EOVal
{
    double number;
    struct Letter letter;
    char none;
};

struct EquationObject
{
    enum EOType type;
    union EOVal value;
};
#endif

void remove_eo_idx(struct EquationObject* list, int length, int idx);
// Make sure list has enough space, pre-increment
void insert_eo_idx(struct EquationObject* list, int length, int idx,
                   struct EquationObject object);
// Quotient is returned in term 0
int div_terms(struct EquationObject* term0, int t0_len,
              const struct EquationObject* term1, int t1_len);
int get_polynomial_degree(const struct EquationObject* expression, int length);
int eo_to_string(const struct EquationObject* expression, int length, char* buffer);