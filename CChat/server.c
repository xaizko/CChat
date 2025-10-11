#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Check for proper usage
    if (argc < 4) {
	fprintf(stderr, "Usage: %s <address> <port> <max clients>\n", argv[0]);
	return 1;
    }

    // Initialize necessary variables
    const char *ADDRESS = argv[1];
    const int PORT = atoi(argv[2]);
    const int MAX_CLIENTS = atoi(argv[3]);


    // Create socket
    const int server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server_file_descriptor < 0) {
	fprintf(stderr, "Failed to create socket\n");
	close(server_file_descriptor);
	return 1;
    }
}
