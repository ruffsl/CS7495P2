
#include "Extractor.h"
#include <iostream>
#include "clustering.hpp"

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
	extractor.findCorrespondingGPSCoords();
	
	vector<int> responsabilities;
	vector<mat> data;
	for (vector<double> i : extractor.correspondingGPS)
	{
	  mat sample;
	  sample << i[0] << i[1] << endr;
	  cout << sample << endl;
	  data.push_back(sample);
	}
	
	GaussianMixtureModel gmm = clustering(data, 10, 4, PENALTY_BIC,
					      responsabilities);

	


	return 0;
}

void Usage(const char* prog)
{
	cout << "Usage: " << prog << " <video file> <locations file>" << endl;
	exit(1);
};
