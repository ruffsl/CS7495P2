#include "Image.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <iostream>

using namespace std;

namespace cs7495
{
	Image::Image() : cv::Mat()
	{
		latitude = -1.0f;
		longitude = -1.0f;
		timestamp = "";
	};

	Image::Image(double latitude, double longitude, string& timestamp) : cv::Mat()
	{
		this->latitude = latitude;
		this->longitude = longitude;
		this->timestamp = timestamp;
	};

	Image::Image(const string& filename, double latitude, double longitude, string timestamp, int flags) :
		latitude(latitude), longitude(longitude), timestamp(timestamp)
	{
		cv::Mat tmp = cv::imread(filename, flags);
		tmp.copyTo(*this);
	};

	void Image::setGPS(double lattitute, double longitude)
	{
		this->latitude = latitude;
		this->longitude = longitude;
	};

	void Image::setTimeStamp(string& timestamp)
	{
		this->timestamp = timestamp;
	};

	void Image::computeSIFT()
	{
		// Resize
		Image tmp(latitude, longitude, timestamp);
		cv::resize(*this, tmp, cv::Size(), 0.5, 0.5, CV_INTER_AREA);
		tmp.copyTo(*this);
		// Compute SIFT
		cv::SIFT sift(0, 3, .1);
		sift(*this, cv::Mat(), keyPoints, descriptors);
	};

	bool Image::writeSIFT2file(const string& filename) const
	{
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
			myfile << keyPoints[i].size << " " << keyPoints[i].angle << " ";
			// Write descriptor
			const float *p = descriptors.ptr<float>(i);
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

	void Image::write(const string& filename)
	{
		cv::imwrite(filename, *this);
	};
}
