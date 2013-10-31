#include <time.h>
#include "Extractor.h"
#include <iostream>
#include "clustering.hpp"
#include <cstdio>

using namespace cs7495;
using namespace std;
using namespace arma;

int main(int argc, char **argv) {
  // Extract
  Extractor extractor;
  extractor.readInfoFromTxt(argv[1]);
  
  vector<int> responsabilities;
  vector<mat> data;
  for (auto i : extractor.getImgInfo()) {
     mat sample;
     sample << i.GPScoord[0] << i.GPScoord[1] << endr;
     data.push_back(sample);
  }
  vector<mat> clusters = locations(100, data);
  
  return 0;
}
