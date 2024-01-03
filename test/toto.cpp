#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <vector>
#include <sys/epoll.h>

int main(int argc, char *argv[])
{
	struct addrinfo hints, *res;
	struct sockaddr_storage their_addr;
	struct sockaddr_in *ipv4; 
	struct epoll_event event;
	char ipstr[INET6_ADDRSTRLEN];
	void *addr;
	socklen_t addr_size;
	const int enable = 1;
	int size;
	int status;
	int sock, sock_accept;
	int fd_epoll;
	int epoll_wait_ret;
	std::vector<char> buff_recv(1);

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
		return (1);
	}
	
	std::cout << "IP addresses for " << argv[1] << std::endl << std::endl;


	ipv4 = (struct sockaddr_in *)res->ai_addr; // cast sockaddr -> sockaddr_in
	addr = &(ipv4->sin_addr);

	inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr); // optional -> translate ip to string
	std::cout << "  IPv4: " << ipstr << std::endl;
	std::cout << res->ai_protocol << std::endl;

	//socket: (PF_INET || AF_INET (same) ; SOCK_STREAM(TCP) ; id protocol)
		// -> set by getaddrinfo
		// return -1 - error ; valid fd(server socket) - success
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock == -1)
	{
		std::cerr << "Socket Error: " << strerror(errno) << std::endl;
		return (1);
	}

	//setsockopt: (server socket ; SOL_SOCKET(enable manipulation socket) ; 
		// REUSEADDR(Bind call should allow reuse same address)
		// REUSEPORT(Allow socket to be bound to same address))
			// return -1 - error ; 0 - success
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1 && setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) == -1)
	{
		std::cerr << "Setsock Error: " << strerror(errno) << std::endl;
		return (1);
	}

	//bind: (server socket ; struct sockaddr with address info ; lenght in bytes of address)
		// -> set by getaddrinfo
		// return -1 - error ; 0 - success
	if (bind(sock, res->ai_addr, res->ai_addrlen) == -1)
	{
		std::cerr << "Bind Error: " << strerror(errno) << std::endl;
		return (1);
	}

	//listen: (server socket ; nb connection allowed in queue)
		// return -1 - error ; 0 - success
	if (listen(sock, 5) == -1)
	{
		std::cerr << "Listen Error: " << strerror(errno) << std::endl;
		return (1);
	}

	addr_size = sizeof(their_addr);
	//accept: (server socket ; storage struct ; size of storage struct)
		//return -1 - error ; valid fd(client socket) - success
	sock_accept = accept(sock, (struct sockaddr *)&their_addr, &addr_size);
	if (sock_accept == -1)
	{
		std::cerr << "Accept Error: " << strerror(errno) << std::endl;
		return (1);
	}

	//epoll_create: (size(ignored but must be greater than one))
		//return -1 - error ; valid fd - sucess
	fd_epoll = epoll_create(1);
	if (fd_epoll == -1)
	{
		std::cerr << "Epoll_create error" << std::endl;
		return (1);
	}

	event.events = EPOLLIN;
	event.data.fd = sock_accept;

	if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, sock_accept, &event) == -1)
	{
		std::cerr << "Epoll_ctl error" << std::endl;
		return (1);
	}

	while (1)
	{
		epoll_wait_ret = epoll_wait(fd_epoll, &event, 10, -1);
		if (epoll_wait_ret == -1)
		{
			std::cerr << "Epoll_wait fail" << std::endl;
			return (1);
		}

		if (event.data.fd ==  sock_accept)
		{
			if ((size = recv(sock_accept, buff_recv.data(), buff_recv.size(), 0)) == -1)
			{
				std::cerr << "Recv Error: " << strerror(errno) << std::endl;
				return (1);
			}
		}
		else
		{
			//send: (client socket, message, length(message), flags)
				//return -1 - error ; number of byte sent - success
			if (send(sock_accept, buff_recv.data(), buff_recv.size(), 0) == -1)
			{
				std::cerr << "Send Error: " << strerror(errno) << std::endl;
				return (1);
			}
		}
	}
	close(sock_accept);
	close(sock);
	freeaddrinfo(res); // free the linked list

	return 0;
}
