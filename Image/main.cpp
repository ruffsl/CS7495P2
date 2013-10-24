#include "Extractor.h"
#include "clustering.hpp"
#include <iostream>

using namespace cs7495;
using namespace std;

void Usage();

int main(int argc, char** argv)
{
	if (argc != 3)
	  Usage();

	Extractor extractor;
	extractor.getTimeName(argv[argc-2]);
	extractor.video2images(argv[argc-2]);
	extractor.readGeoData(argv[argc-1]);
	
        std::vector<float*> coord = extractor.getGPScoord();
	for(int i = 0; i < coord.size(); i++) {
	  cout << coord[i][0] << " " << coord[i][1] << endl;
	}
	return 0;
}

void Usage()
{
	std::cout << "Usage: CS7495P2.exe <video file> <location file>" << std::endl;
	exit(1);
};
