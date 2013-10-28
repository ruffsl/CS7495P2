#include "Image.h"
#include "utils.h"
//#define BOOST_DATE_TIME_NO_LIB
#include <boost/date_time/local_time/local_time.hpp>

using namespace boost::local_time;
using namespace std;

namespace cs7495
{
	/// <summary>
	/// Class to extract frames from a video and associate
	/// a GPS location from a KML file.
	/// </summary>
	class Extractor
	{
	public:
	  vector<Image> frames;				///< Contains all the frames in the video
	  local_date_time *firstFrameTime;	///< Reference time stamp (when recording started)
	  vector<local_date_time> timestamps;	///< List of time stamps written as strings, e.g. "2013-10-19 17:32:43.537"
	  vector<vector<double> > GPScoord;	///< List of GPS coordinates written as an array of two floating point numbers, e.g. {33.782714000, -84.395934000}

	public: unsigned int findFirstTimeStamp();

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Extractor();

		~Extractor();

		/// <summary>
		/// Extract all frames from a video file.
		/// </summary>
		/// <param name="filepath">[in] Path to the video file</param>
		//void breakVideo(const std::string& filepath);

		/// <summary>
		/// Extract each frame and saves it as a JPEG file
		/// without keeping it in memory.
		/// </summary>
		/// <param name="filepath">[in] Path to the video file</param>
		void video2images(const std::string& filepath);


		/// <summary>
		/// Extract time from and MP4 file name
		/// </summary>
		/// <param name="filepath">[in] Path to the video file</param>
		void getTimeName(const std::string& filepath);

		/// <summary>
		/// Open a text file containing time stamps and GPS coordinates
		/// and read them in.
		/// </summary>
		/// <param name=""filepath">[in] Path to the text file</param>
		void readGeoData(const std::string& filepath);

		void findCorrespondingGPSCoords();
	};
}
