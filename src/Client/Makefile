CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm
TARGET = client


SRCS = $(wildcard *.c) ../../lib/cJSON/cJSON.c ../joueur.c ../partie.c ../plateau.c ../regles.c ../utils.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
