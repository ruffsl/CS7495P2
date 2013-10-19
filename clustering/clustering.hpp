#ifndef _CLUSTERING__
#define _CLUSTERING__

#include <vector>
#include <armadillo>
#include "gmm.hpp"

#define PENALTY_NONE 0 // Log Lokilihood
#define PENALTY_BIC  1 // Log Likelihood - param log(|data|)

/**
   Monte Carlo Cross Validation creating one fold
 **/
void mccv(std::vector<arma::mat> data, double percentageTest, 
	  std::vector<arma::mat> &train,
	  std::vector<arma::mat> & test);

/**
   Will cluster the data using a Gaussian Mixture Model,
   the "right" k will be found using cross validation on 
   likelihood or BIC
 **/
GaussianMixtureModel clustering(std::vector<arma::mat> data, int n_folds, int max_k, int penalty,
				std::vector<int> responsability);


#endif
