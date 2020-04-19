/*
 * LINUX implementation of system calls
 */
#include "vproxy.h"
#include "vutils.h"

#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <cstring>

#include <signal.h>

#define BUFFSIZE 2000

//used for logging
std::ofstream log_file;

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

int configure_dev(struct ifreq* ifr, const char* ip)
{
	int sock;
   
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{ 
		perror("Failed to open socket");
		return -1;
	}
   
	if(ioctl(sock, SIOCSIFFLAGS, ifr) < 0)
	{ 
		perror("Cannot set flags");
		close(sock);
		return -1;
	}

	assemble_sockaddr((struct sockaddr_in*)&ifr->ifr_addr, ip, -1);

	if(ioctl(sock, SIOCSIFADDR, ifr) < 0)
	{
		perror("Cannot set IP address. ");
		close(sock);
		return -1;
	}

	close(sock);
	return 0;
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

	if( configure_dev(&ifr, ip) < 0)
	{
		perror("Failed interface configuration");
		close(fd);
		return -1;
	}

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
	uchar buffer[BUFFSIZE];
	uint packet_counter = 0;
	
	while(1)
	{
		nread = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
		
		if(nread < 0)
		{
			perror("error reading from interface");
			close(fd);
			break;
		} 
		
		const std::string& info = assemble_packet_info(buffer, nread, packet_counter);
		std::cout << info;
		
		if(log_file.is_open())
		{
			log_file << info;
		}
		
		packet_counter++;
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

void open_log_file()
{
	log_file.open("log.txt", std::ios::out | std::ios::trunc);

	if(!log_file.is_open())
	{
		std::cout << "Failed to open log file" << std::endl;
	}
	else
	{
		std::cout << "Initialized log file" << std::endl;
	}
}

void close_log_file() { log_file.close(); }
