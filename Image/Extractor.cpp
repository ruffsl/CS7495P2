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

	void Extractor::video2images(const string& filepath)
	{
		if (timestamps.empty())
		{
			cerr << "Error: No GPS locations and time stamps on record! Aborting reading video..." << endl;
			return;
		}
		// Time of first frame
		local_date_time frameTime = *firstFrameTime;
		// Counter on the number of frames
		int counter = 0;
		// Open video file
		cv::VideoCapture cap(filepath);
		// If successful
		if (cap.isOpened())
		{
			cout << "Reading every frame of " << filepath << endl;
			// For all frames
			while (true)
			{
				// 1. Read next frame
				Image tmp;
				// If no next frame, then break
				if (!cap.read(tmp)) break;

				// 2. Find corresponding time stamp and, therefore, GPS coordinates
				// Initialize to first
				boost::posix_time::time_duration duration = *firstFrameTime - timestamps[0];
				long min = abs( duration.total_milliseconds() );
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
			cerr << "Error: could not open " << filepath << endl;
		}
	};

	void Extractor::getTimeName(const string& filepath)
	{
		cout << "Parsing time of first frame." << endl;
		// Get file name
		std::vector<std::string> vect1 = split(filepath, '/');
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
#ifdef DEBUG
		cout << "Time of the first frame:" << endl;
		cout << "\tLocal time:\t"  << firstFrameTime->local_time() << endl;
		cout << "\tTime zone:\t"   << firstFrameTime->zone_as_posix_string() << endl;
		cout << "\tZone abbrev:\t" << firstFrameTime->zone_abbrev() << endl;
		cout << "\tZone offset:\t" << firstFrameTime->zone_abbrev(true) << endl;
#endif
	};

	void Extractor::readGeoData(const string& filepath)
	{
		// Open text file
		ifstream myfile(filepath.c_str());
		if (!myfile.is_open())
		{
			cerr << "Could not open " << filepath << endl;
			return;
		}
		cout << "Parsing " << filepath << " for time stamps and GPS locations." << endl;
		string line;
		// Parse line by line
		while ( getline(myfile, line) )
		{
			// Skip empty lines
			if ( line.compare("") == 0)
				continue;
			// Tokenize line
			vector<string> tokens = split(line, ' ');
			// If line starts with 'T'
			if (tokens[0].compare("T") == 0)
			{
				// 1. Add time stamp
				string timestamp = "";
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

				// 2. Add GPS coordinates
				vector<double> coord;
				coord.push_back(atof(tokens[3].c_str()));
				coord.push_back(atof(tokens[4].c_str()));
				GPScoord.push_back(coord);
			}
		}
	};
}
