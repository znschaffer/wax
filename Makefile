BIN = wax
CFLAGS=-g -Wall $(shell pkg-config --cflags menu ncurses taglib_c)
LDFLAGS= $(shell pkg-config --libs menu ncurses taglib_c)

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $^ $(CFLAGS) -o $@


clean:
	rm $(OBJECTS) $(BIN).o $(BIN)
