#include "headers.h"
#include "httpd.h"
#include "http_request.h"

// threads
void* serverThread(void *vargp);
void* clientThread(void *vargp);
void helloFunction();
void responseCallback(ResponseData response_data);

ClientSettings client_settings;
MessageModel *messages[MAX_MESSAGES_COUNT];
int messages_count = 0;

int main(int argc, char** argv) {

    helloFunction();

    // -- run server --
    ServerTData server_data;
    strcpy(server_data.port, argv[1]);

    pthread_create(&(server_data.tid), NULL, serverThread, (void*)&server_data);

    // -- run client --
    ClientTData client_data;

    pthread_create(&(client_data.tid), NULL, clientThread, (void*)&client_data);

    pthread_join(client_data.tid, NULL);

    clearMessages();

    return 0;
}

// server

void* serverThread(void *vargp) {
    ServerTData server_data = *((ServerTData*)vargp);
    serve_forever(server_data.port);

    pthread_exit(0);
}

void route() {
    MessageModel *msg = (MessageModel*)malloc(sizeof(MessageModel));
    ROUTE_START()

    ROUTE_POST("/message")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");

        char *param;
        param = strstr(qs, "sender");
        param = strtok(param, "=");
        param = strtok(NULL, "");
        strcpy(msg->username, param);

        memcpy( (void*)msg->text, (void*)payload, payload_size );
        msg->text[payload_size] = '\0';
        addMessage(msg);
        printUserspace();
    }

    ROUTE_END()
}

// client
void* clientThread(void *vargp) {
    char str[MAX_MESSAGE_LENGTH];
    RequestData request_data;
    strcpy(request_data.host, client_settings.host);
    request_data.port = client_settings.port;
    strcpy(request_data.type, "POST");
    strcpy(request_data.page, "/message");
    printUserspace();
    while (1) {
        fgets(str, MAX_MESSAGE_LENGTH, stdin);
        sprintf(request_data.data, "%s", str);
        sprintf(request_data.query_params, "?sender=%s", client_settings.username);

        MessageModel *msg = (MessageModel*)malloc(sizeof(MessageModel));
        strcpy(msg->username, "You");
        strcpy(msg->text, str);
        addMessage(msg);
        printUserspace();

        sendTextData(request_data, responseCallback);
    }

    pthread_exit(0);
}

void helloFunction() {
    printf("\n--- Hello user! This is Tiny Messenger created by Sergey B. ---\n\nEnter your login: ");
    scanf("%s", client_settings.username);

    char host[64];

    printf("Enter host: ");
    scanf("%s", host);

    char *rest = host;
    strcpy(client_settings.host, strtok_r(rest, ":", &rest));
    client_settings.port = atoi(strtok_r(rest, ":", &rest));

    getchar();
}

void responseCallback(ResponseData response_data) {
    //printf("%s\n", response_data.data);
}

// messages

void addMessage(MessageModel *message) {
    messages[messages_count] = message;
    messages_count++;
}

void clearMessages() {
    for (int i = 0; i < messages_count; i++) {
        free(messages[i]);
    }
}

void printUserspace() {
    fprintf(stderr, "\033[1;1H\033[2J");
    int row_max = romax();
    int start = messages_count - (row_max - 2);
    start = start < 0 ? 0 : start;

    for (int i = start; i < messages_count; i++) {
        fprintf(stderr, "\x1b[32m%s\x1b[0m: %s", messages[i]->username, messages[i]->text);
    }
    fprintf(stderr, "\033[%d;1H\x1b[33m%s\x1b[0m: ", row_max, "You");
}

int comax() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return(w.ws_col);
}

int romax() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return(w.ws_row);
}
