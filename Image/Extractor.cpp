#include "Extractor.h"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>
#include <limits>
#include <boost/filesystem/operations.hpp>

typedef std::numeric_limits<double> dbl;

//#define DEBUG

namespace cs7495
{
	vector<vector<double> > Extractor::getGPScoord() const
	{
		if (GPScoord.empty()) cout << "Warning: No GPS coordinates on record!" << endl;
		return GPScoord;
	};

	Extractor::Extractor()
	{
		firstFrameTime = new local_date_time(not_a_date_time);
	};

	Extractor::~Extractor()
	{
		delete firstFrameTime;
	};

    void Extractor::video2images(const string& filepath, ofstream& list, bool sift_jpeg)
	{
    	cout << "Creating .sift files for each frame..." << endl;
		if (timestamps.empty())
		{
			cerr << "Error: No GPS locations and time stamps on record! Aborting reading video..." << endl;
			return;
		}
#ifdef DEBUG
		ofstream debug_log("debug_log_video2images.txt");
		debug_log.precision(dbl::digits10);
#endif
		// Time of first frame
		local_date_time frameTime = *firstFrameTime;
		// Counter on the number of frames
		int counter = 0;
		// Open video file
		cv::VideoCapture cap(filepath);
		// If successful
		if (cap.isOpened())
		{
		  //cout << "Reading every frame of " << filepath << " and saving SIFT and GPS data..." << endl;
			// For all frames
			while (true)
			{
				cout << "\r" << counter << " frames treated.        " << flush;
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
					if ( abs(diff.total_milliseconds()) < min)
					{
						min = diff.total_milliseconds();
						index++;
					}
					// If the difference becomes <0 then we passed
					// the pair we were looking for. stop
					if ( diff.total_milliseconds() < 0 )
						break;
				}
				tmp.setGPS(GPScoord[index][0], GPScoord[index][1]);
//				tmp.setTimeStamp(timestamps[index]);

				// 4. Create sub directory
				vector<string> tok = split(filepath, '/');
				tok = split(tok[tok.size()-1], '.');
				boost::filesystem::path subdir(tok[0].c_str());
				boost::filesystem::create_directory(subdir);

				// 5. Extract SIFT
				tmp.computeSIFT();
				if (sift_jpeg)
				{
					stringstream st;
					st << tok[0] << "/" << tok[0] << "_" << counter << "_sift.jpg";
					tmp.showSIFT().write(st.str());
				}

				// 6. Write to a text file
				stringstream ss;
				ss.precision(dbl::digits10);
				ss << tok[0] << "/" << tok[0] << "_" << counter << "_" << fixed << GPScoord[index][0] << "_" << GPScoord[index][1] << ".sift";
				if (!tmp.writeSIFT2file(ss.str()))
					cerr << "\nError in Extractor::video2images(): could not open " << ss.str() << ". Skipping frame." << endl;
				list << ss.str() << endl;
#ifdef DEBUG
				debug_log << "Time: " << frameTime.local_time() << ", GPS: " << fixed << GPScoord[index][0] << ", " << GPScoord[index][1] << endl;
#endif
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
#ifdef DEBUG
		debug_log.close();
#endif
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
#ifdef DEBUG
		ofstream debug_log("debug_log_readGeoData.txt");
		debug_log.precision(dbl::digits10);
#endif
		// Open text file
		ifstream myfile(filepath.c_str());
		if (!myfile.is_open())
		{
			cerr << "Could not open " << filepath << endl;
			return;
		}
		// cout << "Parsing " << filepath << " for time stamps and GPS locations." << endl;
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
				string format = "%Y-%m-%d %H:%M:%S%f %ZP";
				if (split(tokens[2], '.').size() < 2)
					format = "%Y-%m-%d %H:%M:%S %ZP";
				local_time_input_facet *input_facet = new local_time_input_facet(format.c_str());
				// Read time
				stringstream ss;
				ss.imbue(std::locale(ss.getloc(), input_facet));
				local_date_time ldt(not_a_date_time);
				ss.str(timestamp);
				ss >> ldt;
				if (ldt.is_not_a_date_time())
				{
					cout << "Warning: " << ss << " could not be read correctly." << endl;
					cout << "\tFormat is " << format << endl;
					cout << "\tDate: " << tokens[2] << endl;
				}
				// Insert boost local_time into list
				timestamps.push_back(ldt);

				// 2. Add GPS coordinates
				vector<double> coord;
				coord.push_back(atof(tokens[3].c_str()));
				coord.push_back(atof(tokens[4].c_str()));
				GPScoord.push_back(coord);
#ifdef DEBUG
				debug_log << "Time: " << ldt.local_time() << ", GPS: " << fixed << coord[0] << ", " << coord[1] << endl;
#endif
			}
		}
#ifdef DEBUG
		debug_log.close();
#endif
	};

	void Extractor::extract(const string& videopath, const string& kmlpath, const string& filepath)
	{
		// Get time stamp of the first frame
		getTimeName(videopath);
		// Extract <GPS coordinates, time stamp> pairs
		readGeoData(kmlpath);
		ofstream list(filepath.c_str());
		if (!list.is_open())
		{
			cerr << "Error in Extractor::extract(): could not open " << filepath << endl;
			return;
		}
		// Extract frames, match GPS locations and write to file
		video2images(videopath, list);
		list.close();
	};
}
