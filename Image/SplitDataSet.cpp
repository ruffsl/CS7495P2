#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
	ifstream file(argv[argc-1]);
	if (!file.is_open())
	{
		cout << "Could not open " << argv[argc-1] << endl;
		return -1;
	}
	ofstream odd("list_odd.in");
	ofstream even("list_even.in");
	int counter = 0;
	string line;
	while (getline(file, line))
	{
		if (counter % 2 == 0)
		{
			even << line << endl;
		}
		else
		{
			odd << line << endl;
		}
		counter++;
	}
	file.close();
	odd.close();
	even.close();
	return 0;
}
