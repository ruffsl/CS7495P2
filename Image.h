#include <opencv2\core\core.hpp>

namespace cs7495
{
	/// <summary>
	/// Class to represent an image with its data: the image
	/// itself, GPS location, SIFT features.
	/// </summary>
	class Image : public cv::Mat
	{
	private:
		float lattitude;		///< GPS lattitude
		float longitude;		///< GPS longitude
		unsigned int timestamp;	///< Time stamp in seconds
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Image();

		/// <summary>
		/// Construct an image from file and separate data.
		/// </summary>
		Image(const std::string& filename, float lattitute, float longitude, unsigned int timestamp, int flags = 1);

		/// <summary>
		/// Set GPS coordinates.
		/// </summary>
		/// <param name="lattitude">[in] GPS lattitude</param>
		/// <param name="longitude">[in] GPS longitude</param>
		void setGPS(float lattitute, float longitude);

		/// <summary>
		/// Set time stamp.
		/// </summary>
		/// <param name="timestamp">[in] Time stamp</param>
		void setTimeStamp(unsigned int timestamp);
	};

}