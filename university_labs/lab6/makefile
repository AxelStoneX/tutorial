IDIR=inc
ODIR=bin
SDIR=src
PROG_NAME=$(ODIR)/program
LIBS=-lm -lncurses
_DEPS=Structures.h
_OBJ=interface.o function.o main.o
_SRC = ${_OBJ:.o=.c}
######################################################################
CC=gcc
CFLAGS=-I$(IDIR)

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

all: $(PROG_NAME)

$(PROG_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
.PHONY: dirs

dirs:
	@if [ ! -d $(IDIR) ]; then \
	    mkdir $(IDIR) ;        \
	fi
	@if [ ! -d $(ODIR) ]; then \
	    mkdir $(ODIR) ;        \
	fi
	@if [ ! -d $(SDIR) ]; then \
	    mkdir $(SDIR) ;        \
	fi

clean:
	rm -f $(ODIR)/*.o ./$(PROG_NAME)
