BIN = wax
CFLAGS = -Wall -I/opt/homebrew/opt/ncurses/include 
LDFLAGS =-L/opt/homebrew/opt/ncurses/lib -lmenu -lncurses 
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $^ $(CFLAGS) -o $@


clean:
	rm $(OBJECTS) $(BIN)
