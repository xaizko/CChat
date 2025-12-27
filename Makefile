CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -pthread

SRCS = main.c network/client.c network/protocol.c
OBJS = $(SRCS:.c=.o)

TARGET = bin/cchat

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
