#include "Extractor.h"
#include <opencv2/opencv.hpp>

namespace cs7495
{
	Extractor::Extractor()
	{
	};

	void Extractor::breakVideo(const std::string& filepath)
	{
		cv::VideoCapture cap;
		if (cap.open(filepath))
		{
			while (true)
			{
				// Read next frame
				Image tmp;
				// If no next frame, then break
				if (!cap.read(tmp)) break;
				// Otherwise, add it to the vector of features
				frames.push_back(tmp);
			}
			cap.release();
		}
	};

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
				cv::imwrite(ss.str(), tmp);
				std::cout << "Writing " << ss << "..." << std::endl;
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
}
