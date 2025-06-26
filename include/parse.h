#ifndef PO_OBJ
#define PO_OBJ
enum POType {
    MAKE_TEMP,
    COMBINE
};

enum OpType {
    ADD,
};

union POValue {
    struct {
        unsigned int idx;
        OpType op;
        unsigned short num_args;
        double args[];
    } temp;
};

struct ParseObject {
    enum POType type;
    union POValue value;
};

#endif

// Parses one side of equation, will stop at either an equals sign or an end character
// Returns length of buffer
int parse(struct EquationObject *input, int length, struct EquationObject *buffer,
        int max_length);
