typedef union {
  double d;
  struct {
    unsigned long mantissa : 52;
    unsigned int exponent : 11;
    unsigned int sign : 1;
  } parts;
} double_cast;

double ddouble(double num, int n);
double dhalve(double num, int n);

double double_abs(double num);
double dfloor(double num);
double dceil(double num);
double dround(double num);
double dmodulo(double num, double mod);
