#ifndef _CLUSTERING__
#define _CLUSTERING__
#include <armadillo>
#include <vector>

#include "Extractor.h"

double dist(arma::mat x, arma::mat y);

std::vector<arma::mat> locations(double radius, vector<cs7495::ImgInfo> points);

#endif
