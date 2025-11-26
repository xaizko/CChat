#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define DISCONNECT_MESSAGE "DISCONNECT CLIENT"

/* Global Variables */
int *client_sockets;
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *client_socket_ptr) {
    int client_socket = *(int*)client_socket_ptr;
    char buffer[1024];
    ssize_t bytes_received;
    int connected = 1;

    while(connected) {

	if (strcmp(buffer, DISCONNECT_MESSAGE) == 0) {
	    connected = 0;
	    break;
	}

	bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
	if (bytes_received == -1) {
	    perror("failed to receive client message");
	}

	for (int i = 0; i < client_count; i++) {
	    send(client_sockets[i], buffer, sizeof(buffer), 0);
	}
    }

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

    client_sockets = malloc(MAX_CLIENTS * sizeof(int));
    if (client_sockets == NULL) {
	fprintf(stderr, "Failed to allocate memory for client sockets\n");
	return 1;
    }

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

    while (true) { 
	pthread_mutex_lock(&clients_mutex);
	int is_full = client_count >= MAX_CLIENTS;
	pthread_mutex_unlock(&clients_mutex);

	if (is_full) {
	    printf("Server is full. Waiting for slot to open...\n");
	    sleep(1);
	    continue;
	}

	struct sockaddr_in client_info;
	socklen_t addrlen = sizeof(client_info);
	int client_fd = accept(server_file_descriptor, (struct sockaddr *)&client_info, &addrlen);
	if (client_fd < 0) {
	    fprintf(stderr, "Failed to accept client connection\n");
	    close(server_file_descriptor);
	    return 1;
	}

	// Add client_fd to array of client sockets
	pthread_mutex_lock(&clients_mutex);
	client_sockets[client_count++] = client_fd; // This is a post-increment operator so it evalues client_count first
	pthread_mutex_unlock(&clients_mutex);

	// Spawn thread for client
	pthread_t thread;
	int *pclient = malloc(sizeof(int));
	*pclient = client_fd;
	pthread_create(&thread, NULL, handle_client, pclient);
    }

    // Free memory
    free(client_sockets);
    close(server_file_descriptor);
    return 0;
}
