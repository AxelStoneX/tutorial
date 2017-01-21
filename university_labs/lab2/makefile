IDIR=inc
ODIR=bin
SDIR=src
PROG_NAME=$(ODIR)/program

CC=gcc
CFLAGS=-I$(IDIR)

_DEPS=Matrix.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=functions.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_SRC = ${_OBJ:.o=.c}
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

all: $(PROG_NAME)

$(PROG_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

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
