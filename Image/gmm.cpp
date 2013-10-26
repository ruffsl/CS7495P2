#include "gmm.hpp"
#include "numerics.hpp"

#include <cmath>
#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace arma;
using namespace std;

/**
   ---------------
   Gaussian
   ---------------
 */

double Gaussian::evaluate(mat sample) {
  assert(this -> mean.n_cols == sample.n_cols);

  // compute scaler
  int dim = this -> mean.n_cols;  
  double factor = -(dim/2.0) * lg(2 * M_PI);
  double scaler = 0.0;
  double err = 0.0;
  
  for(int i = 0; i < dim; i++) {
    scaler += lg(sqrt(this -> variance(0, i)));
    err += pow(sample[i] - this -> mean(0, i), 2) / this -> variance(0, i) ;
  }
  
  double ll = factor - scaler - 0.5 * err;
  return ll;
};

void Gaussian::estimate(vector<mat> data) {
  int n = data.size();
  int dim = data[0].n_cols;

  // estimate global mean
  mat mu = zeros(1, dim);
  for(int i = 0; i < n; i++) {
    mu = mu + data[i];
  }
  mu = mu / n;
  
  mat sigma = zeros(1, dim);
  
  // estimate covariance matrix
  for(int i = 0; i < n; i++) {    
    mat err = arma::pow(data[i] - mu, 2);
    sigma = sigma + err;
  }
  sigma = sigma / (n - 1); 
  
  this -> mean = mu;
  this -> variance = sigma;
}

mat Gaussian::get_mean() {
  return this -> mean;
}

mat Gaussian::get_variance() {
  return this -> variance;
}

void Gaussian::set_mean(arma::mat mean) {
  this -> mean = mean;
}
 
void Gaussian::set_variance(arma::mat variance) {
  this -> variance = variance;
}

/**
   ----------------------
   Gaussian Mixture Model
   ----------------------
 */

GaussianMixtureModel::GaussianMixtureModel(int k) {
  this -> components = new Gaussian*[k];
  this -> weights = ones(1, k);
}

void GaussianMixtureModel::setComponent(int i,Gaussian *gaussian) {
  this -> components[i] = gaussian;
}

int GaussianMixtureModel::k() {
  return this -> weights.n_cols;
}

/**
   calculate the joint probability
   of the sample given the model   
   formular (1) in [1] 
*/
double GaussianMixtureModel::evaluate(mat sample) {
  double joint = ZERO;
  int k = this -> weights.size();

  for(int i = 0; i < k; i++) {
    double p = lg(this -> weights[i]) + this -> components[i] -> evaluate(sample);  
    joint = lg_sum(joint, p);
  }
  return joint;
}

void GaussianMixtureModel::expectation(vector<mat> samples, mat &expected) {
  int _k = k();
  int n = samples.size();
  
  for(int i = 0; i < n; i++) {
    double scaler = ZERO;
    // compute expectation
    for(int j = 0; j < _k; j++) {
      expected(i, j) = lg(this -> weights(0, j)) + this -> components[j] -> evaluate(samples[i]);
      scaler = lg_sum(scaler, expected(i, j));
    }
    
    // normalize everything
    for(int j = 0; j < _k; j++) {
      expected(i, j) = expected(i, j) - scaler;
      expected(i, j) = exp(expected(i, j));
    }
  }

}

void GaussianMixtureModel::maximization(vector<mat> samples, mat expected) {
  int _k  = k();
  int n   = samples.size();
  int dim = samples[0].n_cols;  

  for(int i = 0; i < _k; i++) {
    double scaler = 0.0;
    mat mean, variance;    
    mean.zeros(1, dim);			       
    variance.zeros(1, dim);    

    // estimate mean, prior and scaler
    for(int j = 0; j < n; j++) {
      scaler += expected(j, i);
      mean = mean + (samples[j] * expected(j, i));
    }
    mean = mean / scaler;
    
    // estimate variance
    for(int j = 0; j < n; j++) {
      mat err = arma::pow(samples[j] - mean, 2);
      variance = variance + (err * expected(j, i));
    }
    variance = variance / scaler;
        
    // build model
    Gaussian *component = new Gaussian();
    component -> set_mean(mean);
    component -> set_variance(variance);
    
    // update
    this -> weights[i] = scaler / (double) n;
    this -> components[i] = component;
  }

}

double GaussianMixtureModel::score(vector<mat> samples) {
  double _score = 0;
  for(int i = 0; i < samples.size(); i++) {
    _score += evaluate(samples[i]);
  }
  return _score;
}

bool GaussianMixtureModel::convergence(double l, double last_l) {
  return abs((l / last_l) - 1) < CONV_TH;     
}

void GaussianMixtureModel::seed(vector<mat> samples) {
  srand(time(0));
  int _k = k();
  int n = samples.size();

  // pick a sample as the first
  // cluster at random 
  // [2] 2.2 1a
  vector<int> centers;
  centers.push_back(rand() % n);
  
  for(int i = 0; i < _k; i++) {
    vector<double> dist;
    vector<int> index;
    double scaler = 0.0;

    // build sampling probabilities
    for(int j = 0; j < n; j++) {    
      // closest cluster
      double min_dist = DBL_MAX;
      for(int l = 0; l < centers.size(); l++) {

	// do not use self
	if(centers[l] != j) {
	  mat dist = arma::pow(samples[centers[l]] - samples[j], 2);
	  double euc = arma::accu(dist);
	  if(euc < min_dist) {
	    min_dist = euc;
	  }
	}
      }

      // only self in iteration one
      if(min_dist == DBL_MAX) {
	min_dist = 0;
      }

      scaler += min_dist;
      dist.push_back(min_dist);
      index.push_back(j);
    }
    
    // d^2(x) -> p(x)
    for(int j = 0; j < n; j++) {
      double x = dist[j];
      dist[j] /= scaler;
    }
    
    // sample new center according to distance
    // acceptance reject
    double r = ((double) rand() / (RAND_MAX));
    int ri = rand() % n;
    while(r >= dist[ri]) {
      r = ((double) rand() / (RAND_MAX));
      ri = rand() % n;
    }
    centers.push_back(index[ri]);    
  }  
  
  double uniform_weight = 1.0 / k();
  int dim = samples[0].n_cols;
  for(int i = 0; i < centers.size(); i++) {
    Gaussian *component = new Gaussian();
    component -> set_mean(samples[centers[i]]);
    component -> set_variance(ones(1, dim));
    this -> weights[i] = uniform_weight;
    this -> components[i] = component;
  }
}

void GaussianMixtureModel::estimate(vector<mat> samples) {
  //cout << "----------------------------NEW " << endl;
  seed(samples);
  //cout << "/SEED" << endl;
  //cout << samples.size() << " " << k() << endl;
  mat _expectation = zeros<mat>(samples.size(), k());   
  //cout << "/EXP" << endl;
  double last_l = 1;
  for(int j = 0; j < EM_MAX_ITER; j++) {
    //cout << "SCORING " << endl;
    double _score = score(samples);
    
    if(std::isnan(_score)) {
      cout << "NaN" << endl;
      break;
    }
    //cout << "em" << endl;
    expectation(samples, _expectation);
    maximization(samples, _expectation);
    //cout << "/em" << endl;
    //cout << "SCORE: " << _score << endl;
    if(convergence(_score, last_l)) {
      cout << "CONVEGENCE" << endl;
      break;
    }
    //cout << "/SCORE: " << _score << endl;
    last_l = _score;
    cout << "----------------------------" << endl;
  }        

}

int GaussianMixtureModel::argmax(arma::mat sample) {
  double max = ZERO;
  int max_component = -1;
  int k = this -> weights.size();
  
  for(int i = 0; i < k; i++) {
    double p = lg(this -> weights[i]) + this -> components[i] -> evaluate(sample);
    if(p > max) {
      max = p;
      max_component = i;
    }
  }
  return max_component; 
}

Gaussian* GaussianMixtureModel::get_pdf(int i) {
  return this -> components[i];
}

mat GaussianMixtureModel::get_weight() {
  return this -> weights;
}

void GaussianMixtureModel::setPriors(mat priors) {
  this -> weights = priors;
}
