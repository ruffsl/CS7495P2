#ifndef _IMG__
#define _IMG__

#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>
//#define BOOST_DATE_TIME_NO_LIB
//#include <boost/date_time/local_time/local_time.hpp>
//
//using namespace boost::local_time;
using namespace std;

namespace cs7495
{
	/// <summary>
	/// Class to represent an image with its data: the image
	/// itself, GPS location, SIFT features.
	/// </summary>
	class Image : public cv::Mat
	{
	private:
		double latitude;						///< GPS latitude
		double longitude;						///< GPS longitude
		string timestamp;						///< Time stamp
		vector<cv::KeyPoint> keyPoints;			///< SIFT point features
		cv::Mat descriptors;					///< SIFT descriptors

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Image();

		Image(double latitude, double longitude, string& timestamp);

		/// <summary>
		/// Construct an image from file and separate data.
		/// </summary>
		Image(const string& filename, double latitude = -1.0, double longitude = -1.0, string timestamp = string(""), int flags = 1);

		/// <summary>
		/// Set GPS coordinates.
		/// </summary>
		/// <param name="latitude">[in] GPS latitude</param>
		/// <param name="longitude">[in] GPS longitude</param>
		void setGPS(double lattitute, double longitude);

		/// <summary>
		/// Set time stamp.
		/// </summary>
		/// <param name="timestamp">[in] Time stamp</param>
		void setTimeStamp(string& timestamp);

		/// <summary>
		/// Extracts key points and descriptors for the image
		/// using SIFT.
		/// </summary>
		void computeSIFT();

		/// <summary>
		/// Write SIFT key points to a text file.
		/// </summary>
		/// <param name="filename">[in] Path to the file to write</param>
		/// <return>False if file could not be opened, true otherwise.</return>
		bool writeSIFT2file(const string& filename) const;

		/// <summary>
		/// Return the image overlayed with its SIFT key points.
		/// </summary>
		/// <return>Image overlayed with SIFT key points</return>
		Image showSIFT();

		/// <summary>
		/// Wrap OpenCV's imwrite function.
		/// </summary>
		void write(const string& filename);
	};

}

#endif
