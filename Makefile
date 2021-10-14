CC=gcc
CFLAGS=-I.
DEPS = r5disassembler.h
OBJ = r5disassembler.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

r5dis: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)