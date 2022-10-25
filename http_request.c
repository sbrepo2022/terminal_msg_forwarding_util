#include "http_request.h"

int sendTextData(RequestData request_data, void (*callback)(ResponseData)) {	
	struct sockaddr_in serv_addr;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(request_data.port);
	inet_pton(AF_INET, request_data.host, &serv_addr.sin_addr);

	if ( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) {
		close(sockfd);
		fprintf(stderr, "Connection to server error!");
		return -1;
	}
 

	char sendline[MAXLINE + 1], recvline[MAXLINE + 1], out_response[MAXRESPONSE + 1];
	char* ptr;
	size_t n;

	// Form request
	snprintf(sendline, MAXLINE - 1, 
		"%s %s%s HTTP/1.0\r\nHost: %s\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n", 
		request_data.type, request_data.page, request_data.query_params, request_data.host, (unsigned int)strlen(request_data.data));
	strcat(sendline, request_data.data);
	strcat(sendline, "\r\n");

	ResponseData response_data;
	// Write the request
	if (write(sockfd, sendline, strlen(sendline)) >= 0) {
	    // Read the response
	    out_response[0] = '\0';
	    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
	        recvline[n] = '\0';

	        strcat(out_response, recvline);
	    }

	    strcpy(response_data.data, out_response);
	    callback(response_data);
	}

	return 1;
}