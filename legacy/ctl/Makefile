APPLICATION = r2c2ctl

# joystick configuration
STICK ?= /dev/input/js0
REMOTEIP ?= "ff02::1%lowpan0"

BINDIR = $(CURDIR)/bin

CC = gcc
CFLAGS = -std=gnu99 -Wall -pthread
INCLUDES += -I$(CURDIR)/../include/

SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)

.PHONY: all clean run

all: $(APPLICATION)

run: $(APPLICATION)
	./$(APPLICATION) $(STICK) $(REMOTEIP)

$(APPLICATION): $(OBJ)
	gcc $(CFLAGS) -o $(APPLICATION) $^

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

clean:
	rm -f *.o $(APPLICATION)
