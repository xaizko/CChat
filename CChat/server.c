#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define ADDRESS 127.0.0.1
#define PORT 8080

int main() {
    int server_file_descriptor;

    // Create socket
    server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server_file_descriptor < 0) {
	fprintf(stderr, "Failed to create socket\n");
	close(server_file_descriptor);
	return 1;
    }
}
