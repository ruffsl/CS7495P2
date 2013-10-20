#include "Extractor.h"
#include <iostream>

using namespace cs7495;

void Usage();

int main(int argc, char** argv)
{
	if (argc != 2)
		Usage();

	Extractor extractor;
	extractor.getTimeName(argv[argc-1]);
	//extractor.video2images(argv[argc-1]);

	return 0;
}

void Usage()
{
	std::cout << "Usage: CS7495P2.exe <video file>" << std::endl;
	exit(1);
};
