#include "Extractor.h"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>

namespace cs7495
{
	Extractor::Extractor()
	{
	};

	//void Extractor::breakVideo(const std::string& filepath)
	//{
	//	cv::VideoCapture cap;
	//	if (cap.open(filepath))
	//	{
	//		while (true)
	//		{
	//			// Read next frame
	//			Image tmp;
	//			// If no next frame, then break
	//			if (!cap.read(tmp)) break;
	//			// Otherwise, add it to the vector of features
	//			frames.push_back(tmp);
	//		}
	//		cap.release();
	//	}
	//};

	void Extractor::video2images(const std::string& filepath)
	{
		// Counter on the number of frames
		int counter = 0;
		// Open video file
		cv::VideoCapture cap(filepath);
		//cap.open(filepath);
		// If successful
		if (cap.isOpened())
		{
			// For all frames
			while (true)
			{
				// Read next frame
				Image tmp;
				// If no next frame, then break
				if (!cap.read(tmp)) break;
				// Otherwise, save it
				std::stringstream ss;
				ss << "frame_" << counter << ".jpeg";
				tmp.write(ss.str());
				std::cout << "Writing " << ss << "..." << std::endl;
				// Extract SIFT
				tmp.computeSIFT();
				// Save image overlayed with SIFT as well
				Image sift = tmp.showSIFT();
				std::stringstream sss;
				sss << "frame_" << counter << "_sift.jpeg";
				sift.write(sss.str());
				// Increment counter
				counter++;
			}
			cap.release();
		}
		else
		{
			std::cout << "Error: could not open " << filepath << std::endl;
		}
	};


	void Extractor::getTimeName(const std::string& filepath)
	{
		// Get file name
		char slash;
#ifdef _WIN32
		slash = '\\';
#else
		slash = '\/';
#endif
		std::vector<std::string> vect1 = split(filepath, slash);

		// Remove extension
		std::vector<std::string> vect2 = split(vect1[vect1.size()-1], '.');

		// Split
		std::vector<std::string> vect3 = split(vect2[0], '_');

		// Format
		std::string date = vect3[1];
		std::string time = vect3[2];
		
		timestamp.year = (unsigned int) atoi(date.substr(0, 4).c_str());
		timestamp.month = (unsigned int) atoi(date.substr(4, 2).c_str());
		timestamp.day = (unsigned int) atoi(date.substr(6, 2).c_str());
		timestamp.hour = (unsigned int) atoi(time.substr(0, 2).c_str());
		timestamp.min = (unsigned int) atoi(time.substr(2, 2).c_str());
		timestamp.sec = (unsigned int) atoi(time.substr(4, 2).c_str());
	};

	void Extractor::readGeoData(const std::string& filepath)
	{
		// Open text file
		std::ifstream myfile(filepath.c_str());
		if (!myfile.is_open())
			return;

		std::string line;
		// Parse line by line
		while ( std::getline(myfile, line) )
		{
			// Skip empty lines
			if ( line.compare("") == 0)
				continue;
			// Tokenize line
			std::vector<std::string> tokens = split(line, ' ');
			// If line starts with 'T'
			if (tokens[0].compare("T") == 0)
			{
				// Add time stamp
				std::string timestamp = "";
				timestamp += tokens[1] + " " + tokens[2];
				timestamps.push_back(timestamp);
				// Add GPS coordinates
				float coord[2] = {0};
				coord[0] = atof(tokens[3].c_str());
				coord[1] = atof(tokens[4].c_str());
				GPScoord.push_back(coord);
			}
		}
	};
}
