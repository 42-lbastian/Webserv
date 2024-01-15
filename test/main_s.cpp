#include "Sessions.hpp"
#include <unistd.h>

int main()
{
	int i;
	time_t time;

	i = 0;
	Sessions ses;
	time = ses.get_time_t(0, 0, 23, 29, 2, 2024);
	if (time == -1)
		std::cerr << "Error date" << std::endl;
	ses.add("Toto", ses.create_token(), time);
	while (1)
	{
		std::cout << ses.verify("Toto", 1) << std::endl;
		sleep(1);
	}
	return (0);
}
