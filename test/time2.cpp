#include <time.h>
#include <iostream>

int main()
{
	time_t actual;
	std::string str;

	actual = time(0);
	struct tm * curtime = localtime ( &actual );
	str = asctime(curtime);
	std::cout << str << std::endl;
	return (0);
}
