#include "Extractor.h"
#include <iostream>

using namespace cs7495;

void Usage();

int main(int argc, char** argv)
{
	if (argc != 2)
		Usage();

	Extractor extractor;
	//extractor.getTimeName(argv[argc-1]);
	//extractor.video2images(argv[argc-1]);
	extractor.readGeoData("C:\\Users\\Bruno\\Documents\\_SCHOOL\\GeorgiaTech\\2013 Fall\\CS 7495 Computer Vision\\Project 2\\20131019_133249\\Walkingfrom10thtoTSRB.txt");

	return 0;
}

void Usage()
{
	std::cout << "Usage: CS7495P2.exe <video file>" << std::endl;
	exit(1);
};
