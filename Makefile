CC=gcc
CFLAGS= -Wall -Wextra -Werror -Wfloat-equal -ggdb -std=c99
# -save-temps : debugging macros -O2
LFLAGS=
LIBS=-lm
SRCS=beuronia.c neuron.c vmath.c system.c memory.c
OBJS=$(SRCS:%.c=$(OUTDIR)/%.o)
OUTDIR=bin
MAIN=beuronia

.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(OUTDIR)/%.o: %.c makedirs
	$(CC) $(CFLAGS) -c $< -o $@

clean: makedirs
	$(RM) $(OUTDIR)/*.o *~ $(OUTDIR)/$(MAIN)

makedirs:
	@mkdir -p $(OUTDIR)

