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
  extractor.readGeoData(argv[1]);
  
  vector<int> responsabilities;
  vector<mat> data;
  int t = 0;
  for (vector<double> i : extractor.getGPScoord()) {
     mat sample;
     sample << i[0] << i[1] << endr;
     data.push_back(sample);
  }
  vector<mat> clusters = locations(100, data);
  for(int i = 0; i < clusters.size(); i++) {
    printf("L: %2.20f %2.20f\n", clusters[i](0, 0), clusters[i](0,1));
  }
  return 0;
}
