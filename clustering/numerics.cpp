#include "numerics.hpp"

double lg(double x) {
  if(x == 0) {
    return ZERO;
  }
  return log(x);
}

double lg_sum(double log_x, double log_y) {
  if(log_x == ZERO || log_y == ZERO) {    
    if(log_x == ZERO && log_y == ZERO) {
      return ZERO;
    }
    if(log_x == ZERO) {
      // y + 0 = 0
      return log_y;
    } else {
      // x + 0 = 0
      return log_x;
    }
  } else {
    if(log_x > log_y) {
      return log_x + lg(1 + exp(log_y - log_x));
    } else {
      return log_y + lg(1 + exp(log_x - log_y));
    }
  }
}

double lg_exp(double log_x) {
  if (log_x == ZERO) {
    return 0;
  }
  return exp(log_x);
}
