#ifndef PROTOCOL_H
#define PROTOCOL_H 

#include <stddef.h>

#define MAX_MSG_LEN 1024
#define MAX_USERNAME_LEN 32
#define DISCONNECT_MESSAGE "DISCONNECT CLIENT"

typedef  enum {
    MSG_CHAT,
    MSG_JOIN,
    MSG_LEAVE,
    MSG_SYSTEM
} MessageType;

typedef struct {
    MessageType type;
    char username[MAX_USERNAME_LEN];
    char content[MAX_MSG_LEN];
} Message;

// Serialize message to send over network
int protocol_encode(const Message* msg, char* buffer, size_t buf_size);

// Deserialize received data into message
int protocol_decode(const char* buffer, size_t len, Message *msg);

// Format message for display
void protocol_format_display(const Message* msg, char* output, size_t out_size);

#endif
