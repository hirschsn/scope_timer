
CC=clang++-5.0
CFLAGS=--std=c++14 -O3 -g3 -ggdb

SRC=example.cc
HDR=scope_timer.h
BIN=${SRC:.cc=}

all: $(BIN)

$(BIN): %: %.cc $(HDR)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BIN)

.PHONY: all clean