BIN = wax
CFLAGS = -Wall -Werror
LDFLAGS = -lncurses
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $^ $(CFLAGS) -o $@


clean:
	rm $(OBJECTS) $(BIN)
