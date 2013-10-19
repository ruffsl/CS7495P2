#include "clustering.hpp"

using namespace std;
using namespace arma;

int main(int argc, char **argv) {
  vector<mat> data;
  for(int i = 0; i < 100; i++) {
    mat x;
    x << 100  + ((double) rand() / (RAND_MAX)) + 1 << 100 + ((double) rand() / (RAND_MAX)) + 1<< endr;
    data.push_back(x);
    cout << x;
  }

  for(int i = 0; i < 100; i++) {
    mat x;
    x << 1000  + ((double) rand() / (RAND_MAX)) + 1 << 1000 + ((double) rand() / (RAND_MAX)) + 1<< endr;
    data.push_back(x);
    cout << x;
  }
  vector<int> responsability;
  GaussianMixtureModel gmm = clustering(data, 10, 5, PENALTY_BIC, responsability);
  cout << "RESULT: " << gmm.k() << endl;
  for(int i = 0; i < gmm.k(); i++) {
    cout << gmm.get_pdf(i) -> get_mean() << endl;
  }
  return 0;
}
