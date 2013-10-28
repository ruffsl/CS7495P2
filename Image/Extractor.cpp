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
		cout << "Extractor::video2images(const std::string& filepath)" << endl;
		// Time of frame
		local_date_time frameTime = *firstFrameTime;
		// Counter on the number of frames
		int counter = 0;
		// Open video file
		cv::VideoCapture cap(filepath);
		// If successful
		if (cap.isOpened())
		{
			// For all frames
			while (true)
			{
				// 1. Read next frame
				Image tmp;
				// If no next frame, then break
				if (!cap.read(tmp)) break;

				// 2. Find corresponding time stamp and thus GPS coordinates
				// Initialize to first
				cout << "size of timestamps: " << (timestamps.empty() ? 0 : timestamps.size()) << endl;
				boost::posix_time::time_duration duration = *firstFrameTime - timestamps[0];
				cout << "long min = std::abs( duration.total_milliseconds() );" << endl;
				long min = std::abs( duration.total_milliseconds() );
				unsigned int index = 0;
				// Iterate through all saved coordinates/time stamps
				for (auto t : timestamps)
				{
					// Compute time difference
					boost::posix_time::time_duration diff = frameTime - t;
					// Memorize the smallest difference and the pair corresponding to it
					if ( std::abs(diff.total_milliseconds()) < min)
					{
						min = diff.total_milliseconds();
						index++;
					}
					// If the difference becomes <0 then we passed
					// the pair we were looking for. stop
					if ( diff.total_milliseconds() < 0 )
						break;
				}

				// 4. Extract SIFT
				tmp.computeSIFT();

				// 5. Write to a text file
				stringstream ss;
				ss << counter << "_" << GPScoord[index][0] << "_" << GPScoord[index][1] << ".sift";
				if (!tmp.writeSIFT2file(ss.str()))
					cerr << "Error in Extractor::video2images(): could not open " << ss.str() << ". Skipping frame." << endl;

				// Increment time of frame
				frameTime += boost::posix_time::seconds(1);
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
		date += vect3[1] + " " + vect3[2] + " EDT";
		// Set up the input date time format.
		local_time_input_facet *input_facet = new local_time_input_facet("%Y%m%d %H%M%S %ZP");
		// Read time
		std::stringstream ss;
		ss.imbue(std::locale(ss.getloc(), input_facet));
		ss.str(date);
		ss >> *firstFrameTime;

		std::cout << "Full Time:\t"   << firstFrameTime->to_string() << std::endl;
		std::cout << "Local time:\t"  << firstFrameTime->local_time() << std::endl;
		std::cout << "Time zone:\t"   << firstFrameTime->zone_as_posix_string() << std::endl;
		std::cout << "Zone abbrev:\t" << firstFrameTime->zone_abbrev() << std::endl;
		std::cout << "Zone offset:\t" << firstFrameTime->zone_abbrev(true) << std::endl;
	};

	void Extractor::readGeoData(const std::string& filepath)
	{
		// Open text file
		std::ifstream myfile(filepath.c_str());
		if (!myfile.is_open())
		{
			cerr << "Could not open " << filepath << endl;
			return;
		}

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
				timestamp += tokens[1] + " " + tokens[2] + " UTC+4";
				// Set up the input datetime format.
				local_time_input_facet *input_facet = new local_time_input_facet("%Y-%m-%d %H:%M:%S%f %ZP");
				// Read time
				std::stringstream ss;
				ss.imbue(std::locale(ss.getloc(), input_facet));
				local_date_time ldt(not_a_date_time);
				ss.str(timestamp);
				ss >> ldt;
				// Insert boost local_time into list
				timestamps.push_back(ldt);

				//std::cout << "\nFull Time:\t"   << ldt.to_string() << std::endl;
				//std::cout << "Local time:\t"  << ldt.local_time() << std::endl;
				//std::cout << "Time zone:\t"   << ldt.zone_as_posix_string() << std::endl;
				//std::cout << "Zone abbrev:\t" << ldt.zone_abbrev() << std::endl;
				//std::cout << "Zone offset:\t" << ldt.zone_abbrev(true) << std::endl;

				//boost::posix_time::time_duration diff = *firstFrameTime - ldt;
				//std::cout << "Difference (h):\t" << diff.hours() << std::endl;
				//std::cout << "Difference (m):\t" << diff.minutes() << std::endl;
				//std::cout << "Difference (s):\t" << diff.total_seconds() << std::endl;
				//std::cout << "Difference (ms):" << diff.total_milliseconds() << std::endl;

				// Add GPS coordinates
				vector<double> coord;
				coord.push_back(atof(tokens[3].c_str()));
				coord.push_back(atof(tokens[4].c_str()));
				GPScoord.push_back(coord);
			}
		}
	};

	unsigned int Extractor::findFirstTimeStamp()
	{
		// Initialize to first
		long min = std::abs( (*firstFrameTime - timestamps[0]).total_milliseconds() );
		unsigned int index = 0;
		// Iterate through all coordinates/time stamps
		for (auto t : timestamps)
		{
			// Compute time difference
			boost::posix_time::time_duration diff = *firstFrameTime - t;
			// Memorize the smallest difference and the pair corresponding to it
			if ( std::abs(diff.total_milliseconds()) < min)
			{
				min = diff.total_milliseconds();
				index++;
			}
			// If the difference becomes <0 then we passed
			// the pair we were looking for. stop
			if ( diff.total_milliseconds() < 0 )
				break;
		}
		// Return the index corresponding to the first frame
#ifdef _DEBUG
		std::cout << "The first frame has time " << firstFrameTime->to_string() << std::endl;
		std::cout << "The corresponding <GPS,time> pair is:\n\ttime: " << timestamps[index].to_string()
			<< "\n\t gps: " << GPScoord[index][0] << ", " << GPScoord[index][1] << std::endl;
#endif
		return index;
	};

	void Extractor::findCorrespondingGPSCoords()
	{
		// Time of frame
		local_date_time frameTime = *firstFrameTime;
		// For all frames
		int n = 0;
		while(n < 100) // stop when no more frames
		{
			// Initialize to first
			long min = std::abs( (*firstFrameTime - timestamps[0]).total_milliseconds() );
			unsigned int index = 0;
			// Iterate through all coordinates/time stamps
			for (auto t : timestamps)
			{
				// Compute time difference
				boost::posix_time::time_duration diff = frameTime - t;
				// Memorize the smallest difference and the pair corresponding to it
				if ( std::abs(diff.total_milliseconds()) < min)
				{
					min = diff.total_milliseconds();
					index++;
				}
				// If the difference becomes <0 then we passed
				// the pair we were looking for. stop
				if ( diff.total_milliseconds() < 0 )
					break;
			}
			// Return the index corresponding to the first frame
			//correspondingGPS.push_back(GPScoord[index]);
			// Increment time of frame
			frameTime += boost::posix_time::seconds(1);
			n++;
		}
	};
}
