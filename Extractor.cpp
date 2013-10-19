#include "Extractor.h"

#include <opencv2\opencv.hpp>

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
		cv::VideoCapture cap("VID_20131019_133249.mp4");
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
				counter++;
			}
			cap.release();
		}
		else
		{
			std::cout << "Error: could not open " << filepath << std::endl;
		}
	};
}
