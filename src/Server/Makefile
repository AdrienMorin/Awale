CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm
TARGET = server

SRCS = $(wildcard *.c) ../joueur.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
