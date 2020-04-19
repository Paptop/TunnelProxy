#include <iostream>
#include <cstring>

#include "vproxy.h"

int tapfd = 0;
int fds = 0;

void on_signal(int sig)
{
	printf("Received signal %d\n", sig);
	close_fd(fds);
	close_fd(tapfd);
	exit(1);
}

int main(int argc, char** argv)
{
	if(argc <= 1 || argc > 3)
	{
		printf("Wrong usage\n");
		exit(1);
	}

	on_sys_signal(on_signal);

	const char* ip = argv[1];
	const char* port = argv[2];
	const int iPort = atoi(port);

	printf("Opening tap device on %s\n", ip);
  tapfd = open_tap("tap0", ip);

	if(tapfd < 0)
	{
		perror("error opening tap interface");
		exit(1);
	}

	printf("Opening port on tap device %s:%s\n", ip, port);
  fds = open_socket_udp(ip, iPort);	
	read_socket_to_console(fds);

	return 0;
}
