#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

int main()
{
	time_t currentTime = time(NULL);
	std::string res;

    std::stringstream timeStream;
    timeStream << currentTime;
	res = timeStream.str();

	return (0);
}
