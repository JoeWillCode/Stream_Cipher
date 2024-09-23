CC = gcc
CFLAGS = -g -std=c99 -fsanitize=address,undefined

all: encrypt decrypt clean

encrypt: encrypt.c
	$(CC) $(CFLAGS) -o $@ $< -lm

decrypt: decrypt.c
	$(CC) $(CFLAGS) -o $@ $< -lm	

clean:
	rm -f *.o
