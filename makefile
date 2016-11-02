CC=gcc
CFLAGS=-I.
DEPS = main.h showzone.h command.h traverse.h showfile.h
OBJ = main.o  showzone.o traverse.o showfile.o

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

minix: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)
