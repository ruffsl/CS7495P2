
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

void Usage(const char* prog);

int main(int argc, char** argv)
{
	if (argc != 3)
		Usage(argv[0]);

	Extractor extractor;
	// Get time stamp of the first frame
	extractor.getTimeName(argv[argc-2]);
	//extractor.video2images(argv[argc-2]);
	// Extract <GPS coordinates, time stamp> pairs
	extractor.readGeoData(argv[argc-1]);
	// Find pair matching the first frame
	extractor.findCorrespondingGPSCoords();


	Point2 x_initial(0.0, 0.0);
	SharedDiagonal P_initial = 
	  noiseModel::Diagonal::Sigmas(Vector_(2, 0.1, 0.1));

	ExtendedKalmanFilter<Point2> ekf(x_initial, P_initial);
	SharedDiagonal R = noiseModel::Diagonal::Sigmas(Vector_(2, 0.25, 0.25), true);
	Vector u = Vector_(2, 1.0, 0.0);
	SharedDiagonal Q =
	  noiseModel::Diagonal::Sigmas(Vector_(2, 0.1, 0.1), true);	
	
	vector<int> responsabilities;
	vector<mat> data;
	int t = 0;
	for (vector<double> i : extractor.GPScoord) {
	  mat sample;
	  sample << i[0] << i[1] << endr;
	  if(data.size() > 0) {
	    double d = dist(sample, data[data.size() - 1]);
	  }
	  data.push_back(sample);

	  Point2 difference(1,0);
	  Symbol x('x', t);	  
	  Symbol xpp('x', t + 1);	  
	  Point2 z(i[0], i[1]);
	  PriorFactor<Point2> factor(x, z, R);
	  Point2 x_update = ekf.update(factor);
	  x_update.print("X Update: ");

	  BetweenFactor<Point2> factor2(x, xpp, difference, Q);
	  Point2 x_predict = ekf.predict(factor2);
	  x_predict.print("X Predict: ");
	  t++;
	}

	locations(100, data);
	return 0;
}

void Usage(const char* prog)
{
	cout << "Usage: " << prog << " <video file> <locations file>" << endl;
	exit(1);
};
