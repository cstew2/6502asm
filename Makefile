TARGET		= 6502asm

CC		= gcc
CFLAGS		:= 

DFLAGS		= -Wall -Wextra -g -ggdb -O0 -pedantic
RFLAGS		= -O2 -fwhole-program -march=native -pipe -ftree-vectorize -finline-functions -funswitch-loops

LIBS		= 

SRC		= $(wildcard *.c)
OBJ		= $(SRC:.c=.o)

default: debug
debug: CFLAGS += $(DFLAGS)
debug: $(TARGET)

release: CFLAGS += $(RFLAGS)
release: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CLFAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)
