LIBS = -lpthread

all: server

clean:
	@rm -rf *.o
	@rm -rf server

server: main.o httpd.o http_request.o
	gcc -o server $^ $(LIBS)

main.o: main.c httpd.h
	gcc -c -o main.o main.c

httpd.o: httpd.c httpd.h
	gcc -c -o httpd.o httpd.c

http_request.o: http_request.c http_request.h
	gcc -c -o http_request.o http_request.c
