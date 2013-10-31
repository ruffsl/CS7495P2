#include <time.h>
#include "Extractor.h"
#include <iostream>
#include "clustering.hpp"
#include <cstdio>
 
using namespace cs7495;
using namespace std;

int main(int argc, char **argv) {

  // Extract
  Extractor extractor;
  extractor.readInfoFromTxt(argv[1]);  
  int num = 0;
  for(int i = 0; i < extractor.getImgInfo().size(); i++) {
    if(extractor.getImgInfo()[i].GPScoord[0] == 33.782713999999999) {
      if(extractor.getImgInfo()[i].GPScoord[1] == -84.395933999999997) {
	num++;
      }
    }
  }

  cout << num << endl;
  locations(50, extractor.getImgInfo());  

  /**
  arma::mat x;
  arma::mat y;
  x << -80 << 33 << arma::endr;
  y << -80 << 20 << arma::endr;
  cout << dist(x,y) << endl;
  **/
  return 0;
}
