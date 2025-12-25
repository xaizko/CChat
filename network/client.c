#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "client.h"
#include "protocol.h"

static int server_fd = -1;
static int connected = 0;
static pthread_t recv_thread;
static MessageCallback msg_callback = NULL;

static void *receive_loop(void *arg) {
    char buffer[MAX_MSG_LEN];
    Message msg;

    while (connected) {
	ssize_t bytes = recv(server_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0) {
	    connected = 0;
	    break;
	}
	buffer[bytes] = '\0';

	if (protocol_decode(buffer, bytes, &msg) == 0 && msg_callback) {
	    msg_callback(&msg);
	}
    }

    return NULL;
}

int client_connect(const char* address, int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return -1;

    struct sockaddr_in server_addr = {
	.sin_family = AF_INET,
	.sin_port = htons(port),
	.sin_addr.s_addr = inet_addr(address)
    };

    if (connect(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))  != 0) {
	close(server_fd);
	server_fd = -1;
	return -1;
    }

    connected = 1;
    return 0;
}

void client_disconnect() {
    connected = 0;
    if (server_fd >= 0) {
	close(server_fd);
	server_fd = -1;
    }
}

int client_send(const Message* msg) {
    if (!connected) return -1;

    char buffer[MAX_MSG_LEN];
    int len = protocol_encode(msg, buffer, sizeof(buffer));
    return send(server_fd, buffer, len, 0);
}

int client_starT_receiver(MessageCallback callback) {
    msg_callback = callback;
    return pthread_create(&recv_thread, NULL, receive_loop, NULL);
}

int client_is_connected() {
    return connected;
}
