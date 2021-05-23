CC=gcc
CFLAGS= -Wall -Wextra -Werror -Wfloat-equal -Wno-unused-parameter -Wno-unused-variable -ggdb -std=c99
# -save-temps : debugging macros -O2
LFLAGS=
LIBS=-lm
SRCS=beuronia.c _node.c vmath.c system.c memory.c neuron.c link.c network.c layer.c
OBJS=$(SRCS:%.c=$(OUTDIR)/%.o)
OUTDIR=bin
MAIN=beuronia

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	(cd ./tests && make clean && make)

$(OUTDIR)/%.o: %.c makedirs
	$(CC) $(CFLAGS) -c $< -o $@

clean: makedirs
	$(RM) $(OUTDIR)/*.o *~ $(OUTDIR)/$(MAIN) $(TARGETS)

makedirs:
	@mkdir -p $(OUTDIR)

