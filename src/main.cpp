#include <iostream>
#include <cstring>

#include "vproxy.h"

int tapfd = 0;
int fds = 0;

void clean()
{
	close_fd(fds);
	close_fd(tapfd);
	close_log_file();
}

void on_signal(int sig)
{
	printf("Received signal %d\n", sig);
	clean();
	exit(1);
}

int main(int argc, char** argv)
{
	if(argc <= 1 || argc > 3)
	{
		printf("usage: %s [ tun/tap interface ip ] [udp port]\n", argv[0]); 
		exit(1);
	}

	open_log_file();
	on_sys_signal(on_signal);

	const char* ip = argv[1];
	const char* port = argv[2];
	const int iPort = atoi(port);

	printf("Opening tap device on %s\n", ip);
	tapfd = open_tap("tap0", ip);

	if(tapfd < 0)
	{
		perror("Error opening tap interface");
		clean();
		exit(1);
	}

	printf("Opening port on tap device %s:%s\n", ip, port);
	fds = open_socket_udp(ip, iPort);	
	read_socket_to_console(fds);

	clean();
	return 0;
}
