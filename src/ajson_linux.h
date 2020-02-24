#ifndef PRO_LINUX_H
#define PRO_LINUX_H

#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/rsa.h>
#include <sys/socket.h>
#include <sys/sem.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <net/if.h>
#include <ifaddrs.h>

/*
#define STR_SIZE0 16
#define STR_SIZE1 32
#define STR_SIZE2 64
#define STR_SIZE3 128
#define STR_SIZE4 256
#define STR_SIZE5 512

typedef enum { REQ_COMPLETED, REQ_TOO_LARGE, REQ_INCOMPLETE } req_status_t;

//#define printf my_printf

int my_printf(const char *format, ...);
*/

#endif
