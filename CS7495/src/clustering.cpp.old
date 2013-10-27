#include "clustering.hpp"
#include "numerics.hpp"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>    // std::find
#include <iostream>

using namespace std;
using namespace arma;

/**
   Monte Carlo Cross Validation creating one fold
 **/
void mccv(vector<mat> data, double percentageTest, 
	  vector<mat> &train,
	  vector<mat> &test) {
  train.clear();
  test.clear();
  vector<int> index;
  int size =  data.size() * percentageTest;
  for(int i = 0; i < size; i++) {
    int num = rand() % data.size();
    test.push_back(data[num]);
    index.push_back(num);
  }

  for(int i = 0; i < data.size(); i++) {
    if(find(index.begin(), index.end(), i) == index.end()) {
      train.push_back(data[i]);
    }
  }

}

/**
   Will cluster the data using a Gaussian Mixture Model,
   the "right" k will be found using cross validation on 
   likelihood or BIC
 **/
GaussianMixtureModel clustering(vector<mat> data, int n_folds, int max_k, int penalty,
				vector<int> responsability) {
  
  double *avg_score_k       = new double[max_k];
  double *max_score_k       = new double[max_k];
  GaussianMixtureModel *max = new GaussianMixtureModel[max_k];

  for(int i = 0; i < max_k; i++) {
    avg_score_k[i] = 0;
    max_score_k[i] = ZERO;
  }
  avg_score_k[0] = ZERO;

  for(int i = 0; i < n_folds; i++) {
    vector<mat> train;
    vector<mat> test;
    mccv(data, 0.5, train, test);    
    cout << "FOLD: " << train.size() << " " << test.size() << endl;

    for(int k = 1; k < max_k; k++) {
      GaussianMixtureModel gmm(k);
      gmm.estimate(train);
      double ll = gmm.score(test);
      if(penalty == PENALTY_BIC) {
	ll -= ((k * 3) / 2) * lg(test.size());
      }
      cout << "    " << k << " " << ll << endl;
      avg_score_k[k] += ll;
      if(ll > max_score_k[k]){
	max_score_k[k] = ll;
	max[k] = gmm;
      }
    }    
  }

  double max_k_final = ZERO;
  int k = -1;
  for(int i = 1; i < max_k; i++) {
    avg_score_k[i] /= n_folds;
    cout << "i: " << avg_score_k[i] << endl;
    if(avg_score_k[i] > max_k_final) {
      max_k_final = avg_score_k[i];
      k = i;
    }
  }
  
  for(int i = 0; i < data.size(); i++) {
    responsability.push_back(max[k].argmax(data[i]));
  }

  return max[k];
}

