#ifndef _HTTP_REQUEST___
#define _HTTP_REQUEST___

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 2000
#define MAXQUERY 1000
#define MAXRESPONSE 10000

typedef struct {
	char host[128];
	int port;
	char type[64];
	char page[128];
	char query_params[MAXQUERY];
	char data[MAXLINE];
} RequestData;

typedef struct {
	char data[MAXRESPONSE];
} ResponseData;

int sendTextData(RequestData request_data, void (*callback)(ResponseData));

#endif
