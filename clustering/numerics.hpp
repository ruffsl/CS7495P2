#ifndef _LOG_NUMERICS__
#define _LOG_NUMERICS__

/**
   Numerical calculations:
   
   [1] Thomas Mann: Numerically Stable Hidden Markov Model Implementation

   by Daniel Kohlsdorf
   mailto dkohlsdorf@gmail.com
 **/


#include <cmath>
#include <cfloat>
#include <armadillo>

// log(0)
const double ZERO = -1 * DBL_MAX;

/**
   Compute a numerically stable log
 */
double lg(double x);

/**
   Compute the numerically stable sum
   of two logs
 */
double lg_sum(double log_x, double log_y);

/**
   Compute numerically stable exponential
   of a log value.
 */
double lg_exp(double log_x);

#endif
