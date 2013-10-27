#include "utils.h"
#include <sstream>

namespace cs7495
{
	std::vector<std::string> split(const std::string& s, char delim)
	{
		std::vector<std::string> elems;
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim))
		{
			// No empty string
			if (item.compare("") != 0)
				elems.push_back(item);
		}
		return elems;
	};
}
