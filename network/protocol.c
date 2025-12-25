#include <stdio.h>
#include <string.h>
#include "protocol.h"

int protocol_encode(const Message* msg, char* buffer, size_t buf_size) {
    // format: "TYPE:USERNAME:CONTENT"
    return snprintf(buffer, buf_size, "%d:%s:%s",
		    msg->type, msg->username, msg->content);
}

int protocol_decode(const char* buffer, size_t len, Message *msg) {
    int type;
    if (sscanf(buffer, "%d:%31[^:]:%1023[^\n]",
	       &type, msg->username, msg->content) == 3) {
	msg->type = (MessageType)type;
	return 0;
    }
    return -1;
}

void protocol_format_display(const Message* msg, char* output, size_t out_size) {
    switch(msg->type) {
	case MSG_CHAT:
	    snprintf(output, out_size, "[%s]: %s", msg->username, msg->content);
	    break;
	case MSG_JOIN: 
	    snprintf(output, out_size, "*** %s joined the chat ***", msg->username);
	    break;
	case MSG_LEAVE: 
	    snprintf(output, out_size, "*** %s left the chat ***", msg->username);
	    break;
	case MSG_SYSTEM:
	    snprintf(output, out_size, "[SERVER]: %s", msg->content);
	    break;
    }
}
