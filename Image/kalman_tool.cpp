#include <time.h>
#include "Extractor.h"
#include <iostream>
#include "clustering.hpp"
#include <cstdio>
#include <gtsam/nonlinear/ExtendedKalmanFilter.h>
#include <gtsam/nonlinear/Symbol.h>
#include <gtsam/slam/PriorFactor.h>
#include <gtsam/slam/BetweenFactor.h>
#include <gtsam/geometry/Point2.h>

using namespace cs7495;
using namespace std;
using namespace arma;
using namespace gtsam;

typedef Point2 LinearMeasurement;

// initialize at file boundaries
int reinit_at[3] = {
  528,
  1125,
  2083
};

int main(int argc, char** argv) {

	// Record time
	time_t start_time = clock(); 

	Point2 x_initial(0, 0);
	SharedDiagonal P_initial = noiseModel::Diagonal::Sigmas(Vector_(2, 
						   0.1, 0.1));
	// Create an ExtendedKalmanFilter object
	ExtendedKalmanFilter<Point2> ekf(x_initial, P_initial);
	
	// R: obs
	SharedDiagonal Q = noiseModel::Diagonal::Sigmas(Vector_(2, .9,
								   .9), true);
	// Q: trans
	SharedDiagonal R = noiseModel::Diagonal::Sigmas(Vector_(2, .1, .01), true);
	Point2 lastSeen;
	Point2 difference(0.1, 0.1);
	
	ifstream myReadFile;
	myReadFile.open(argv[1]);
	char output[100];
	int file = 1;
	double total = 0;
	double total_f = 0;
	int init = -1;
	if (myReadFile.is_open()) {	  
	  double cur[4];
	  int num = 0;
	  while (!myReadFile.eof()) {	        
	    //if(file > 520) {
	    //  break;
	    //}
	    myReadFile >> output;
	    double val = -1;
	    string nop("-");
	    string out(output);
	    if(out.compare(nop) != 0) {
	      val = atof(output);
	    }
	    
	    cur[num] = val;
	    
	    if(num == 3) {
	      mat x, y;

	      if(file == 1) {
		Point2 _x_initial(cur[0], cur[1]);
		SharedDiagonal _P_initial = 
		  noiseModel::Diagonal::Sigmas(Vector_(2, .1, .1));
		
		ExtendedKalmanFilter<Point2> _ekf(_x_initial, _P_initial);		
		ekf = _ekf;
	      }
	      
	      x << cur[0] << cur[1] << endr;
	      y << cur[2] << cur[3] << endr;	      

	      double d = dist(x, y);
	      
	      Symbol xp('x',  file);	  
	      Symbol xpp('x', file + 1);	  
	      Point2 z(cur[0], cur[1]);	      

	      BetweenFactor<Point2> factor2(xp, xpp, difference, Q);
	      Point2 x_predict = ekf.predict(factor2);
	      mat pred, obs;
	      pred << x_predict.x() << x_predict.y() << endr;
	      obs  << cur[2] << cur[3] << endr;	  	      
	      
	      if(cur[0] != -1) {
		PriorFactor<Point2> factor(xp, z, R);
		Point2 x_update = ekf.update(factor);	      
		difference = (z - lastSeen) / 2;
		lastSeen = z;
	      }
	      
	      // tracker needs some time before tracking
	      if(file > 100) {
		total += d;
		total_f += dist(pred, obs);
		cout << dist(pred, obs) << ", " << d << endl;
	      }
	      num = 0;
	      file++;
	      if(file == reinit_at[init]) {
		file = 1;
		init++;
	      }
	    } else  {	    
	      num++;
	    }
	  }
	  
	}
	myReadFile.close();

	return 0;
};

