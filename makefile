CC = g++
CFLAGS = -Wall -pedantic -g

all: ca.cpp
	$(CC) $(CFLAGS) -o ca ca.cpp
clean:
	rm -rf *.o
