.PHONEY : clean all

CC = gcc
CFLAGS = -Wall -g -pthread
BIN = pthread_create pthread_clean

all : $(BIN)

%.o : %.c
	$(cc) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(BIN)
