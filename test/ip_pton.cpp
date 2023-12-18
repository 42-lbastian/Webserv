#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

int main()
{
	struct sockaddr_in sa;

	inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr)); // IPv4
	std::cout << sa.sin_addr.s_addr << std::endl;

	return (0);
}
