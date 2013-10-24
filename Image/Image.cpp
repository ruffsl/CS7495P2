#include "Image.h"
#include <opencv2/highgui/highgui.hpp>

namespace cs7495
{
	Image::Image() : cv::Mat()
	{
		latitude = -1.0f;
		longitude = -1.0f;
		timestamp = -1;
	};

	Image::Image(const std::string& filename, float latitude, float longitude, time_t timestamp, int flags) :
		latitude(latitude), longitude(longitude), timestamp(timestamp)
	{
		cv::Mat tmp = cv::imread(filename, flags);
		tmp.copyTo(*this);
	};

	void Image::setGPS(float lattitute, float longitude)
	{
		this->latitude = latitude;
		this->longitude = longitude;
	};

	void Image::setTimeStamp(time_t timestamp)
	{
		this->timestamp = timestamp;
	};

	void Image::computeSIFT()
	{
		cv::SIFT sift;
		sift(*this, cv::Mat(), keyPoints, descriptors);
	};

	Image Image::showSIFT()
	{
		Image out;
		cv::drawKeypoints(*this, keyPoints, out);
		return out;
	};

	void Image::write(const std::string& filename)
	{
		cv::imwrite(filename, *this);
	};
}
