#include <stdio.h>
#include <stdlib.h>

// Socket Libraries
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
	fprintf(stderr, "Usage: %s <server address> <port>", argv[0]);
	return 0;
    }
    
    // Initialize Info
    const char *ADDRESS = argv[1];
    const int PORT = atoi(argv[2]);

    int server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server_file_descriptor < 0) {
	perror("Failed to create socket for server\n");
	return -1;
    }

    struct sockaddr_in socket_info;
    socket_info.sin_family = AF_INET;
    socket_info.sin_port = htons(PORT);
    socket_info.sin_addr.s_addr = inet_addr(ADDRESS);
}
