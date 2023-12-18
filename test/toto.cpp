#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>

int main(int argc, char *argv[])
{
	struct addrinfo hints, *res;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
	void *addr;
	struct sockaddr_in *ipv4; 
	int sock;

	if (argc != 2) {
		fprintf(stderr,"usage: showip hostname\n");
		return (1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; //AF_INET = IPv4
	hints.ai_socktype = SOCK_STREAM; //SOCK_STREAM = TCP
	//to look hints.ai_flags = AI_PASSIVE;

	//getaddrinfo: (host name (ip or name) ; port ; struct base ; struct result)
		// return status code - error ; 0 - success
	if ((status = getaddrinfo(argv[1], "42424", &hints, &res)) != 0) {
		//gai_strerror translate error
		std::cerr << "getaddrinfo:" << gai_strerror(status) << std::endl; 
		return (2);
	}
	
	std::cout << "IP addresses for " << argv[1] << std::endl << std::endl;


	ipv4 = (struct sockaddr_in *)res->ai_addr; // cast sockaddr -> sockaddr_in
	addr = &(ipv4->sin_addr);

	inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr); // optional -> translate ip to string
	std::cout << "  IPv4: " << ipstr << std::endl;
	std::cout << res->ai_protocol << std::endl;

	//socket: (PF_INET || AF_INET (same) ; SOCK_STREAM(TCP) ; id protocol)
		// -> set by getaddrinfo
		// return -1 - error ; valid fd - success
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock == -1)
	{
		std::cout << "Socket Error: " << strerror(errno) << std::endl;
		return (3);
	}

	//bind: (socket ; struct sockaddr with address info ; lenght in bytes of address)
		// -> set by getaddrinfo
		// return -1 - error ; 0 - success
	if (bind(sock, res->ai_addr, res->ai_addrlen) == -1)
	{
		std::cout << "Bind Error: " << strerror(errno) << std::endl;
		return (4);
	}

	
	

	freeaddrinfo(res); // free the linked list

	return 0;
}
