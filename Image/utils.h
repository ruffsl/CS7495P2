#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>

namespace cs7495
{
	/// <summary>
	/// Structure for a time stamp.
	/// </summary>
	struct TimeStamp
	{
		unsigned int year;
		unsigned int month;
		unsigned int day;
		unsigned int hour;
		unsigned int min;
		unsigned int sec;
	};

	/// <summary>
	/// Helper function to split a string.
	/// </summary>
	/// <param name="s">[in] Input string</param>
	/// <param name="delim">[in] Delimitator</param>
	/// <return>Vector a strings</return>
	std::vector<std::string> split(const std::string& s, char delim);
}



#endif /* UTILS_H_ */
