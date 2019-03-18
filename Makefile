CC = gcc
CFLAGS  = -lm -Wall -ansi -pedantic -Wextra -g -pg
#CFLAGS += -D_POSIX_C_SOURCE=200112L
#CFLAGS += -D_FILE_OFFSET_BITS=64
CFLAGS += -std=c89

#CFLAGS += -D_BIG_RAM

LFLAGS  =
DEPS =
OBJ = hamming.o
OUT_EXE = hamming_tool


all:$(OUT_EXE)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(OUT_EXE): $(OBJ)
	$(CC) $(LFLAGS) $^ -o $@


.PHONY:clean install rebuild
clean:
	rm -f *.o $(OUT_EXE)

install:
	cp ./$(OUT_EXE) /bin

rebuild: clean
	 make
