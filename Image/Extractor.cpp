#include "Extractor.h"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>

namespace cs7495
{
	Extractor::Extractor()
	{
		firstFrameTime = new local_date_time(not_a_date_time);
	};

	Extractor::~Extractor()
	{
		delete firstFrameTime;
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
		std::string date = "";
		date += vect3[1] + " " + vect3[2];
		// Set up the input date time format.
		local_time_input_facet *input_facet = new local_time_input_facet("%Y%m%d %H%M%S");
		// Read time
		std::stringstream ss;
		ss.imbue(std::locale(ss.getloc(), input_facet));
		ss.str(date);
		ss >> *firstFrameTime;

		//std::cout << "Local time:\t"  << firstFrameTime->local_time() << std::endl;
		std::cout << "Time zone:\t"   << firstFrameTime->zone_as_posix_string() << std::endl;
		std::cout << "Zone abbrev:\t" << firstFrameTime->zone_abbrev() << std::endl;
		std::cout << "Zone offset:\t" << firstFrameTime->zone_abbrev(true) << std::endl;

		delete input_facet;
	};

	void Extractor::readGeoData(const std::string& filepath)
	{
		// Set up the input datetime format.
		local_time_input_facet *input_facet = new local_time_input_facet("%Y-%m-%d %H:%M:%S%f");

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
				// Read time
				std::stringstream ss;
				ss.imbue(std::locale(ss.getloc(), input_facet));
				local_date_time ldt(not_a_date_time);
				ss.str(timestamp);
				ss >> ldt;
				// Insert boost local_time into list
				timestamps.push_back(ldt);

				// Add GPS coordinates
				float coord[2] = {0};
				coord[0] = atof(tokens[3].c_str());
				coord[1] = atof(tokens[4].c_str());
				GPScoord.push_back(coord);
			}
		}
		delete input_facet;
	};
}
