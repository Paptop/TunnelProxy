#ifndef VPROXY_H
#define VPROXY_H 

int open_tap(const char* dev, const char* ip);
int open_socket_udp(const char* ip, int port);

void read_socket_to_console(int fd);

void close_fd(int fd);

void on_sys_signal(void (*handler)(int));

void open_log_file();
void close_log_file();

#endif //VPROXY_H
