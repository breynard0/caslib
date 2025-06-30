#include "../include/equation_objects.h"
#include <stdio.h>

void print_eo(struct EquationObject object) {
  switch (object.type) {
  case NUMBER:
    printf("Number: %0.16f", object.value.number);
    break;
  case LETTER:
    printf("Letter: %c{%c}", object.value.letter.letter,
           object.value.letter.subscript);
    break;
  case MULT:
    printf("Mult");
    break;
  case DIV:
    printf("Div");
    break;
  case ADD:
    printf("Add");
    break;
  case SUB:
    printf("Sub");
    break;
  case PI_VAL:
    printf("Pi");
    break;
  case ROOT:
    printf("Root");
    break;
  case EXP:
    printf("Exp");
    break;
  case BLOCK_START:
    printf("Block start");
    break;
  case BLOCK_END:
    printf("Block end");
    break;
  case EQUAL:
    printf("Equal");
    break;
  case SINE:
    printf("Sine");
    break;
  case COSINE:
    printf("Cosine");
    break;
  case TANGENT:
    printf("Tangent");
    break;
  case ARCSINE:
    printf("ArcSine");
    break;
  case ARCCOSINE:
    printf("ArcCosine");
    break;
  case ARCTANGENT:
    printf("ArcTangent");
    break;
  case LOG:
    printf("Logarithm");
    break;
  }
  printf("\n");
}
