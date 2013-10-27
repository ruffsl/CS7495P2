#include "Extractor.h"
#include <iostream>

using namespace cs7495;
using namespace std;

void Usage(const char* prog);

int main(int argc, char** argv)
{
	if (argc != 3)
		Usage(argv[0]);

	Extractor extractor;
	// Get time stamp of the first frame
	extractor.getTimeName(argv[argc-2]);
	// Extract <GPS coordinates, time stamp> pairs
	extractor.readGeoData(argv[argc-1]);
	// Extract frames, match GPS locations and write to file
	extractor.video2images(argv[argc-2]);
	// Find pair matching the first frame
//	extractor.findCorrespondingGPSCoords();
//	for (auto i : extractor.correspondingGPS)
//	{
//		printf("%f %f\n", i[0], i[1]);
//	}

	return 0;
}

void Usage(const char* prog)
{
	cout << "Usage: " << prog << " <video file> <locations file>" << endl;
	exit(1);
};
