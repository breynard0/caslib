#include "../include/objects.h"
#include <stdio.h>

void print_eo(struct EquationObject object) {
  switch (object.type) {
  case NUMBER:
    printf("Number: ");
  case LETTER:
    printf("Letter: ");
  case MULT:
    printf("Mult");
  case DIV:
    printf("Div");
  case ADD:
    printf("Add");
  case SUB:
    printf("Sub");
  case PI_VAL:
    printf("Pi");
  case ROOT:
    printf("Root: ");
  case EXP:
    printf("Exp");
  case BLOCK_START:
    printf("Block start");
  case BLOCK_END:
    printf("Block end: ");
  case EQUAL:
    printf("Number: ");
  case SINE:
    printf("Number: ");
  case COSINE:
    printf("Number: ");
  case TANGENT:
    printf("Number: ");
  case ARCSINE:
    printf("Number: ");
  case ARCCOSINE:
    printf("Number: ");
  case ARCTANGENT:
    printf("Number: ");
  case LOG:
    printf("Number: ");
    break;
  }
  printf("\n");
}
