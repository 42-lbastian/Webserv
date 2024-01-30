#include "Sessions.hpp"
#include <unistd.h>

int main()
{
	int i;
	time_t time;
	std::string token;
	std::string token2;

	i = 0;
	Sessions ses;
	time = ses.get_time_t(0, 0, 23, 29, 2, 2024);
	if (time == -1)
		std::cerr << "Error date" << std::endl;
	token = ses.create_token();
	ses.add(token, time);
	token2 = ses.create_token();
	std::cout << token << token2 << std::endl;
	ses.print_ses();
	while (1)
	{
		std::cout << ses.verify(token) << std::endl;
		sleep(1);
	}
	return (0);
}
