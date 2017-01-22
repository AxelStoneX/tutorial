IDIR=inc
ODIR=bin
SDIR=src
PROG_NAME=$(ODIR)/program
LIBS=-lm
_DEPS=thrown_body.h types.h
_OBJ=thrown_body.o main.o
_SRC = ${_OBJ:.o=.cpp}
######################################################################
CC=g++
CFLAGS=-I$(IDIR)

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

all: $(PROG_NAME)

$(PROG_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
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
