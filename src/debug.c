#include "equation_objects.h"
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
  case END_LEX:
    printf("End Lex");
    break;
  case NONE:
    break;
  }
  printf("\n");
}

void print_eo_flat(struct EquationObject object) {
  switch (object.type) {
  case NUMBER:
    printf("%lli", (long long)object.value.number);
    break;
  case LETTER:
    printf("%c", object.value.letter.letter);
    break;
  case MULT:
    printf("*");
    break;
  case DIV:
    printf("/");
    break;
  case ADD:
    printf("+");
    break;
  case SUB:
    printf("-");
    break;
  case PI_VAL:
    printf("π");
    break;
  case ROOT:
    printf(":(");
    break;
  case EXP:
    printf("^");
    break;
  case BLOCK_START:
    printf("(");
    break;
  case BLOCK_END:
    printf(")");
    break;
  case EQUAL:
    printf("=");
    break;
  case SINE:
    printf("sin");
    break;
  case COSINE:
    printf("cos");
    break;
  case TANGENT:
    printf("tan");
    break;
  case ARCSINE:
    printf("asin");
    break;
  case ARCCOSINE:
    printf("acos");
    break;
  case ARCTANGENT:
    printf("atan");
    break;
  case LOG:
    printf(":(");
    break;
  case END_LEX:
    printf("\n");
    break;
  case NONE:
    break;
  }
}
