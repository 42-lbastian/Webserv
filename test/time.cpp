#include <unistd.h>
#include <iostream>
#include <time.h>

/*
struct tm end {
	.tm_min = 48,
	.tm_hour = 17,
	.tm_mday = 11,
	.tm_mon = 0,
	.tm_year = 2024 - 1900
};
*/

int main()
{
	time_t futur;
	time_t actual;
	tm tm = {};
	int i;

	tm.tm_min = 5;
	tm.tm_hour = 18;
	tm.tm_mday = 11;
	tm.tm_mon = 0;
	tm.tm_year = 2024 - 1900;

	i = 0;
	futur = mktime(&tm);
	while (1)
	{
		std::cout << i << std::endl;
		actual = time(0);
		if (actual > futur)
			break;
		sleep(1);
		i++;
	}
	return (0);
}
