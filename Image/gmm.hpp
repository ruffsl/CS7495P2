#ifndef _DISTRIBUTION__
#define _DISTRIBUTION__

#include <vector>
#include <armadillo>

#define CONV_TH 1e-6
#define EM_MAX_ITER 300

/**
   A collection of probability distributions
   used as observation probabilities
   
   by Daniel Kohlsdorf
   mailto dkohlsdorf@gmail.com
 **/

class Distribution {
public:

  virtual double evaluate(arma::mat sample) = 0;
  virtual void estimate(std::vector<arma::mat> data) = 0;

};

/**
 * A Multivariate Gaussian
 * defined by its mean (1 x DIM)
 * and variance matrix (DIM x DIM)
 *
 * [1] Wendy Holmes: Speech Synthesis and Recognition, 2nd ed.
 * 
 */ 
class Gaussian : public Distribution {
public:

  /**
     compute N(x; mu, sigma)
   **/
  double evaluate(arma::mat sample);
  
  /**
     estimate sample mean and sample variance
   **/
  void estimate(std::vector<arma::mat> data);

  arma::mat get_mean();  
  arma::mat get_variance();
  void set_mean(arma::mat mean);  
  void set_variance(arma::mat variance);


private:
  arma::mat variance;
  arma::mat mean;
};

/**
   Implements a gaussian mixture model

   [1] NIKOS VLASSIS and ARISTIDIS LIKAS:
   "A Greedy EM Algorithm for Gaussian Mixture
   Learning", Neural Processing Letters 15, 
   77 - 87, 2002   

   [2] David Arthur, Sergei Vassilvitskii:
   "k-means++: The Advantage of Careful Seeding",
   ACM - SIAM, 2007

 **/
class GaussianMixtureModel : public Distribution {
public:
  GaussianMixtureModel() { /* DEFAULT */};

  GaussianMixtureModel(int k);
  
  int k();
  
  /**
   calculate the joint probability
   of the sample given the model   
   formular (1) in [1] 
  */
  double evaluate(arma::mat sample);
  
  /**
     Find the maximum component
   **/
  int argmax(arma::mat sample);

  /**
     compute the expected values P(xi|muj, sigma_j)   
     formular (4) in [1] 
   */
  void expectation(std::vector<arma::mat> samples, arma::mat &expected);
  
  /**
     maximize the parameters using the expectation
     formular (5-7) in [1] 
   */
  void maximization(std::vector<arma::mat> samples, arma::mat expected);

  /**
     Score of a data set
     given the current model
     formular (3) in [1] 
   */
  double score(std::vector<arma::mat> samples);
  
  /**
     did the algorithm converge?
     abs (l / last_l -1) > 1e-6     
     Section (3.3) in [2] 
   */
  bool convergence(double l, double last_l); 

  /**
   K - Means++ seeding. Will be O(log k)
   competetive. [2]
  */
  void seed(std::vector<arma::mat> samples);

  /**
   Run expectation maximization
  */
  void estimate(std::vector<arma::mat> samples);
  
  void setPriors(arma::mat priors);
  void setComponent(int i,Gaussian *gaussian);
  Gaussian* get_pdf(int i);
  arma::mat get_weight();
private:      
  Gaussian **components;
  arma::mat weights;  
};

#endif
