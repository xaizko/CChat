#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "network/client.h"
#include "network/protocol.h"

static volatile int running = 1;

// callback invoked when a message is received from server
void on_message_received(const Message* msg) {
    char display[MAX_MSG_LEN + MAX_USERNAME_LEN + 20];
    protocol_format_display(msg, display, sizeof(display));
    printf("%s\n", display);
}

void handle_sigint(int sig) {
    running = 0;
}

int main(int argc, char* argv[]) {
    // TODO: add error checking and customizable
    const char* server_ip = "127.0.0.1";
    const int port = 8080;
    char username[MAX_USERNAME_LEN] = "User"; 

    signal(SIGINT, handle_sigint);

    if (client_connect(server_ip, port) != 0) {
	fprintf(stderr, "Failed to connect to %s:%d\n", server_ip, port);
	return 1;
    }
    printf("Connected to server!\n");

    // Start receiver thread
    client_start_receiver(on_message_received);

    // Join message
    Message join_msg = {
	.type = MSG_JOIN
    };
    strncpy(join_msg.username, username, MAX_USERNAME_LEN -1);
    client_send(&join_msg);

    // Main input loop
    char input[MAX_MSG_LEN];
    while (running && client_is_connected()) {
	if (fgets(input, sizeof(input), stdin) != NULL) {
	    input[strcspn(input, "\n")] = '\0';

	    if (strcmp(input, "/quit") == 0) break;
	    
	    Message chat_msg = {
		.type = MSG_CHAT
	    };
	    strncpy(chat_msg.username, username, MAX_USERNAME_LEN - 1);
	    strncpy(chat_msg.content, input, MAX_MSG_LEN - 1);
	    client_send(&chat_msg);

	}
    }

    // Leave msg
    Message leave_msg = {
	.type = MSG_LEAVE
    };
    strncpy(leave_msg.username, username, MAX_USERNAME_LEN - 1);
    client_send(&leave_msg);

    client_disconnect();
    printf("Disconnected.\n");
    return 0;
}
