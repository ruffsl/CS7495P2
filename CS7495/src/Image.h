#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>

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
		float latitude;							///< GPS latitude
		float longitude;						///< GPS longitude
		time_t timestamp;						///< Time stamp in seconds
		vector<cv::KeyPoint> keyPoints;			///< SIFT point features
		cv::Mat descriptors;					///< SIFT descriptors

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Image();

		/// <summary>
		/// Construct an image from file and separate data.
		/// </summary>
		Image(const string& filename, float latitude = -1.0f, float longitude = -1.0f, time_t timestamp = -1, int flags = 1);

		/// <summary>
		/// Set GPS coordinates.
		/// </summary>
		/// <param name="latitude">[in] GPS latitude</param>
		/// <param name="longitude">[in] GPS longitude</param>
		void setGPS(float lattitute, float longitude);

		/// <summary>
		/// Set time stamp.
		/// </summary>
		/// <param name="timestamp">[in] Time stamp</param>
		void setTimeStamp(time_t timestamp);

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
		bool writeSIFT2file(const string& filename);

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
