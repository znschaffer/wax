BIN = wax
CFLAGS=-g -Wall $(shell pkg-config --cflags menu ncurses taglib_c) 
LDFLAGS= $(shell pkg-config --libs menu ncurses taglib_c)
PREFIX = /usr/local
BINDIR = bin
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $^ $(CFLAGS) -o $@

clean:
	rm -f $(OBJECTS) $(BIN).o $(BIN) $(BIN).log

.PHONY: install
install:
	mkdir -p $(DESTDIR)$(PREFIX)/$(BINDIR)
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/$(BINDIR)

.PHONY: uninstall
uninstall:
	rm  $(DESTDIR)$(PREFIX)/$(BINDIR)/$(BIN)

