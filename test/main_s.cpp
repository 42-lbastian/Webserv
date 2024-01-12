#include "Sessions.hpp"
#include <unistd.h>

int main()
{
	time_t futur;
	tm tm = {};
	int i;

	tm.tm_min = 48;
	tm.tm_hour = 18;
	tm.tm_mday = 11;
	tm.tm_mon = 0;
	tm.tm_year = 2024 - 1900;

	i = 0;
	futur = mktime(&tm);

	Sessions ses;
	ses.add("Toto", ses.create_token(), futur);
	while (1)
	{
		std::cout << ses.verify("Toto", 1) << std::endl;
		ses.print();
		sleep(1);
	}
	return (0);
}
