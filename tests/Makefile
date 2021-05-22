CC=gcc
CFLAGS= -Wall -Wextra -Werror -Wfloat-equal -O2 -std=c99 -I../
# -save-temps : debugging macros -O2 -Wno-unused-variable -ggdb
LFLAGS=
LIBS=-lm
TESTS=$(wildcard *.c)
TARGETS=$(patsubst %.c,%,$(TESTS))
OBJS=$(filter-out ../bin/beuronia.o, $(wildcard ../bin/*.o))

all: $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -c $< -o $@.o
	$(CC) $(CFLAGS) -o $@ $@.o $(OBJS) $(LFLAGS) $(LIBS)
	$(RM) $@.o

clean:
	$(RM) $(TARGETS)


