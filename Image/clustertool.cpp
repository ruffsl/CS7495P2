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
  locations(20, extractor.getImgInfo());
  
  return 0;
}
