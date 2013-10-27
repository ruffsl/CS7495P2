
#include "Extractor.h"
#include <iostream>
#include "clustering.hpp"
#include <cstdio>

using namespace cs7495;
using namespace std;
using namespace arma;

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
	// extractor.findCorrespondingGPSCoords();
	
	vector<int> responsabilities;
	vector<mat> data;
	for (vector<double> i : extractor.GPScoord)
	{
	  mat sample;
	  sample << i[0] << i[1] << endr;
	  if(data.size() > 0) {
	    double d = dist(sample, data[data.size() - 1]);
	    printf("%2.6f %2.6f %2.6f\n", sample(0, 0), sample(0,1), d);
	  }
	  data.push_back(sample);
	}

	locations(100, data);
	return 0;
}

void Usage(const char* prog)
{
	cout << "Usage: " << prog << " <video file> <locations file>" << endl;
	exit(1);
};
