#ifndef CLIENT_H
#define CLIENT_H

#include "protocol.h"

// Callback for when a message is received 
typedef void (*MessageCallback)(const Message* msg);

// Connect to server
int client_connect(const char *address, int port);

// Disconnect from server
void client_disconnect();

// Send a message 
int client_send(const Message* msg);

// Start receiving thread, calls callback for each message 
int client_start_receiver(MessageCallback callback);

// Check if connected
int client_is_connected(void);

#endif
