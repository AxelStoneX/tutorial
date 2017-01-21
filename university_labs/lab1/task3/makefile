IDIR=inc
ODIR=bin
SDIR=src
PROG_NAME=program

CC=gcc
CFLAGS=-I$(IDIR)

_DEPS=TAS.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=functions.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_SRC = ${_OBJ:.o=.c}
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

all: $(PROG_NAME)

program: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o ./$(PROG_NAME)
