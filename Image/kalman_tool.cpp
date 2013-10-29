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

int main(int argc, char** argv) {

	// Record time
	time_t start_time = clock();

	// Extract
	cout << "Extracting GPS locations and SIFT features..." << endl;
	Extractor extractor;
	extractor.readGeoData(argv[1]);

	Point2 x_initial(0.0, 0.0);
	SharedDiagonal P_initial = 
	  noiseModel::Diagonal::Sigmas(Vector_(2, 0.1, 0.1));

	ExtendedKalmanFilter<Point2> ekf(x_initial, P_initial);
	SharedDiagonal R = noiseModel::Diagonal::Sigmas(Vector_(2, 0.25, 0.25), true);
	Vector u = Vector_(2, 1.0, 0.0);
	SharedDiagonal Q =
	  noiseModel::Diagonal::Sigmas(Vector_(2, 0.1, 0.1), true);	
	
	int t = 0;
	Point2 lastSeen;
	Point2 difference(0.1, 0.1);
	for (vector<double> i : extractor.getGPScoord()) {	  
	  Symbol x('x', t);	  
	  Symbol xpp('x', t + 1);	  
	  Point2 z(i[0], i[1]);
	  
	  if(t % 10 == 0) {
	    difference = (z - lastSeen) / 10;
	    lastSeen = z;
	    difference.print();
	    PriorFactor<Point2> factor(x, z, R);
	    Point2 x_update = ekf.update(factor);	    
	    x_update.print(">>>>>>>>>>>>>>>>> X Update: ");
	  }
	  BetweenFactor<Point2> factor2(x, xpp, difference, Q);
	  Point2 x_predict = ekf.predict(factor2);
	  mat pred, obs;
	  pred << x_predict.x() << x_predict.y() << endr;
	  obs << i[0] << i[1] << endr;	  
	  cout << "ERR: " << dist(pred, obs) << endl;
	  // x_predict.print("X Predict: ");
	  t++;
	}
	return 0;
};

