CC=gcc
CFLAGS=-I.
DEPS = main.h showzone.h command.h traverse.h
OBJ = main.o  showzone.o traverse.o

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

minix: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)
