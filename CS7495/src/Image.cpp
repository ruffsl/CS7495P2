#include "Image.h"
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>

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
		cout << "Image::computeSIFT()" << endl;
		cv::SIFT sift;
		sift(*this, cv::Mat(), keyPoints, descriptors);
	};

	bool Image::writeSIFT2file(const string& filename)
	{
		cout << "Image::writeSIFT2file(const string& filename)" << endl;
		ofstream myfile(filename.c_str());
		if (!myfile.is_open())
			return false;

		// Write the total number of key points followed
		// by the size of a key point (128)
		myfile << descriptors.rows << " " << descriptors.cols << endl;

		// Write every key point
		for (int i=0; i<descriptors.rows; i++)
		{
			// Write pixel location
			myfile << keyPoints[i].pt.y << " " << keyPoints[i].pt.x << " ";
			// Write scale and orientation
			myfile << keyPoints[i].size << " " << keyPoints[i].angle << endl;
			// Write descriptor
			float *p = descriptors.ptr<float>(i);
			for (int j=0; j<descriptors.cols; j++)
			{
				myfile << p[j] << " ";
			}
			myfile << endl;
		}
		myfile.close();
		return true;
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
