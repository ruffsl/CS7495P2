#ifndef _CLUSTERING__
#define _CLUSTERING__

#include <armadillo>
#include <vector>

double dist(arma::mat x, arma::mat y);

std::vector<arma::mat> locations(double radius, std::vector<arma::mat> points);

#endif
