#include "Split.h"

using namespace std;
using namespace cs7495;

void split(vector<Image> input, vector<Image> &test, vector<Image> &train) {
  test.clear();
  train.clear();
  for(int i = 0; i < input.size(); i++) {
    if(i % 2 == 0) {
      test.push_back(input[i]);
    } else {
      train.push_back(input[i]);
    }
  }
}
