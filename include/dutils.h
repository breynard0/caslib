#ifndef DUTILS_H
#define DUTILS_H

typedef union
{
    double d;

    struct
    {
        // This is a long long because 32 bit
        unsigned long long mantissa : 52;
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
#endif // DUTILS_H