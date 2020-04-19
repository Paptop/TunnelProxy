/*
 * LINUX implementation of system calls
 */
#include "vproxy.h"

#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <signal.h>

#define BUFFSIZE 2000

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

int open_dev(const char* dev, int flags, const char* ip)
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
 
	ifr.ifr_flags = flags | IFF_TAP | IFF_UP;
   
	if((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0)
	{ 
		perror("ioctl(TUNSETIFF)");
		close(fd);
		return err;
	}
 
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

	assemble_sockaddr( (struct sockaddr_in*)&ifr.ifr_addr, ip, -1);

	if(ioctl(sock, SIOCSIFADDR, &ifr) < 0)
	{
		perror("Cannot set IP address. ");
		close(sock);
		exit(1);
	}

	close(sock);

	return fd;
}

int open_tap(const char* dev, const char* ip)
{
	char tap_name[IFNAMSIZ];
	strcpy(tap_name, dev);
	return open_dev(tap_name, IFF_TAP | IFF_UP, ip);
}

int open_socket_udp(const char* ip, int port)
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

	return sfd;
}

void read_socket_to_console(int fd)
{
  int nread = 0;
  char buffer[BUFFSIZE];
  
  while(1)
  {
    nread = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
    if(nread < 0)
    {
      perror("Reading from interface");
      close(fd);
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

void close_fd(int fd) { close(fd); }

void on_sys_signal(void (*handler)(int))
{
	signal(SIGHUP,  handler);
	signal(SIGTERM, handler);
	signal(SIGKILL, handler);
	signal(SIGSTOP, handler);
	signal(SIGINT,  handler);
}
