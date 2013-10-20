#include "Image.h"
#include <opencv2/highgui/highgui.hpp>

namespace cs7495
{
	Image::Image() : cv::Mat()
	{
		lattitude = -1.0f;
		longitude = -1.0f;
		timestamp = -1;
	};

	Image::Image(const std::string& filename, float lattitude, float longitude, unsigned int timestamp, int flags) :
		lattitude(lattitude), longitude(longitude), timestamp(timestamp)
	{
		cv::Mat tmp = cv::imread(filename, flags);
		tmp.copyTo(*this);
	};

	void Image::setGPS(float lattitute, float longitude)
	{
		this->lattitude = lattitude;
		this->longitude = longitude;
	};

	void Image::setTimeStamp(unsigned int timestamp)
	{
		this->timestamp = timestamp;
	};
}
