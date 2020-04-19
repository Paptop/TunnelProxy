#include <iostream>
#include <cstring>

#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 2000

/*
 * char* dev : the name of an interfaces, c ended string
 * int flags : interface flags ( IFF_TUN etc.)
 */

void assemble_sockaddr(struct sockaddr_in* addr, const char* ip, int port)
{
	bzero((char*) addr, sizeof(*addr));
	addr->sin_family = AF_INET;
	
	if(port > 0)
	{
		addr->sin_port = htons(port);	
	}

	inet_aton(ip, &addr->sin_addr);
}

int tun_alloc(char* dev, int flags, const char* ip, int port)
{
	struct ifreq ifr;
	int fd, err;

	const char* clonedev = "/dev/net/tun";

	if( (fd = open(clonedev, O_RDWR)) < 0) 
	{
		perror("Opening /dev/net/tun");
		return fd;
	}

	memset(&ifr, 0, sizeof(ifr));

	if(*dev)
	{
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	}

	ifr.ifr_flags = flags;

	if((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0)
	{
		perror("ioctl(TUNSETIFF)");
		close(fd);
		return err;
	}

	strcpy(dev, ifr.ifr_name);

	int sock;

	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Failed to open socket");
		exit(1);
	}

	if(ioctl(sock, SIOCSIFFLAGS, &ifr) < 0)
	{
		perror("Cannot set flags");
		close(sock);
		exit(1);
	}

	assemble_sockaddr((struct sockaddr_in*)&ifr.ifr_addr, ip, -1);

	if(ioctl(sock, SIOCSIFADDR, &ifr) < 0)
	{
		perror("Cannot set IP address. ");
		close(sock);
		exit(1);
	}

	close(sock);

	return fd;
}


void open_socket(const char* ip, int port)
{
	struct sockaddr_in tun_addr;
	int sfd;

	assemble_sockaddr(&tun_addr, ip, port);

	if((sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("Failed opening socket");
		exit(1);
	}

	if(bind(sfd, (struct sockaddr*) &tun_addr, sizeof(tun_addr)) < 0)
	{
		perror("Failed binding");
		exit(1);
	}

	int nread = 0;
	char buffer[BUFFSIZE];

	while(1)
	{
		nread = recvfrom(sfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
		if(nread < 0)
		{
			perror("Reading from interface");
			close(sfd);
			exit(1);
		}

		printf("\n");
		printf("-------SIZE---------\n");
		printf(" Size: %d bytes \n", nread);
		printf("-------BINARY-------\n");
		for(int i = 0; i < nread; ++i)
		{	
			 printf("%02x:",buffer[i]);

		   if( (i + 1) % 16 == 0 )
			 {
					printf("\n"); 
			 }
		}
		printf("\n");
		printf("-------ASCII-------\n");
		for(int i = 0; i < nread; ++i)
		{
			 printf("%c",buffer[i]);

		   if( (i + 1)  % 16 == 0 && i)
			 {
					printf("\n"); 
			 }
		}
		printf("-------------------\n");
		printf("\n");
	}
}

int main(int argc, char** argv)
{
	char tun_name[IFNAMSIZ];
	char tap_name[IFNAMSIZ];
	char buffer[BUFFSIZE];
	char *a_name;

	if(argc <= 1 || argc > 3)
	{
		printf("Wrong usage\n");
		exit(1);
	}

	const char* ip = argv[1];
	const char* port = argv[2];
	int iPort = atoi(port);

	printf("Output ip %s : %s \n", ip, port);

	int tunfd, tapfd;

	strcpy(tun_name, "tap0");

	printf("Tun_alloc\n");

  //tunfd = tun_alloc(tun_name, IFF_TAP | IFF_UP, ip, iPort);

	/*
	if(tunfd < 0)
	{
		perror("Allocationg interface");
		exit(1);
	}
	*/

  open_socket(ip, iPort);	

	/*
	int nread;

	while(1)
	{
		nread = read(tunfd, buffer, sizeof(buffer));
		if(nread < 0)
		{
			perror("Reading from interface");
			close(tunfd);
			exit(1);
		}

		printf("Read %d bytes from device %s\n", nread, tun_name);
	}
	*/


	return 0;
}
