CC = gcc
CFLAGS = -O2 -Wall
LD = gcc
LDFLAGS = -lncurses
HEADERS = $(shell find src -name "*.h")

OBJS = build/main.o

all: sctp-analyzer

sctp-analyzer: $(OBJS)
	gcc -o sctp-analyzer $(LDFLAGS) $(OBJS)

build/%.o: src/%.c $(HEADERS)
	@mkdir -p build/
	gcc -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	@rm -Rf build/
	@rm -f sctp-analyzer
