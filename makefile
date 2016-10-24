CC=gcc
CFLAGS=-I.
DEPS = main.h
OBJ = main.o  

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

minix: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)