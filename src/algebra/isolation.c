#include "equation_objects.h"
#include "cauchy.h"
#include "bundan.h"
#include "pow.h"

double next_delimiter(struct EquationObject* expression, int length, double offset, double bound) {
  int starting_roots = bundan_max_roots(expression, length, offset, bound);
  
  double delim_guess = (bound - offset) / 2.0;
  double uncertainty = (bound - offset) / 2.0;
  
  while (uncertainty > 0.00000000001) {
    int lower_roots = bundan_max_roots(expression, length, delim_guess - uncertainty + offset, delim_guess + offset);
    int upper_roots = bundan_max_roots(expression, length, delim_guess + offset, delim_guess + uncertainty + offset);
    
    uncertainty /= 2;
    if (lower_roots == 0) {
      delim_guess += uncertainty;
    } else {
      delim_guess -= uncertainty;
    }
    
  }
  
  return delim_guess;
}

int get_isolation_delimiter_positions(struct EquationObject* expression, int length, double* delimiters) {
  int cauchy = get_bound_abs(expression, length);
  int out_len = 0;
  
  delimiters[out_len] = next_delimiter(expression, length, 0.0, cauchy);
  
  return out_len;
}
