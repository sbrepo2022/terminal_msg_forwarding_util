#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ioctl.h>		// чтение данных о нажатой клавише

#define MAX_MESSAGE_LENGTH 1000
#define MAX_MESSAGES_COUNT 1000

// server

typedef struct {
	pthread_t tid;
	char port[7];
} ServerTData;

void* serverThread(void *vargp);

// client

typedef struct {
	pthread_t tid;
} ClientTData;

typedef struct {
	char username[128];
	char host[30];
	int port;
} ClientSettings;

void* clientThread(void *vargp);
void helloFunction();

// messages

typedef struct {
	char text[MAX_MESSAGE_LENGTH];
	char username[128];
} MessageModel;

extern MessageModel *messages[MAX_MESSAGES_COUNT];
extern int messages_count;

void addMessage(MessageModel *message);
void clearMessages();
void printUserspace();
int comax();
int romax();
