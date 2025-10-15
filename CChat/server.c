#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

void *handle_client(void *client_socket_ptr) {
    int client_socket = *(int*)client_socket_ptr;
    free(client_socket_ptr);
    return 0;
}

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

    struct sockaddr_in sock_info;
    memset(&sock_info, 0, sizeof(sock_info));
    sock_info.sin_family = AF_INET;
    sock_info.sin_addr.s_addr = inet_addr(ADDRESS);
    sock_info.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_file_descriptor, (struct sockaddr *)&sock_info, sizeof(sock_info)) < 0) {
	fprintf(stderr, "Failed to bind\n");
	close(server_file_descriptor);
	return 1;
    }

    // Listen for connections 
    if (listen(server_file_descriptor, MAX_CLIENTS) < 0) {
	fprintf(stderr, "Failed to listen for connections\n");
	close(server_file_descriptor);
	return 1;
    }

    int clients = 0;
    while (clients < MAX_CLIENTS) { 
	int client_fd;
	struct sockaddr_in client_info;
	socklen_t addrlen = sizeof(client_info);
	client_fd = accept(server_file_descriptor, (struct sockaddr *)&sock_info, &addrlen);
	if (client_fd < 0) {
	    fprintf(stderr, "Failed to accept client connection\n");
	    close(server_file_descriptor);
	    return 1;
	}

	pthread_t thread;
	int *pclient = malloc(sizeof(int));
	*pclient = client_fd;
	pthread_create(&thread, NULL, handle_client, pclient);
    }
}
