DIR_OUTPUT = ./bin
IDIR =./include
CC = gcc
CFLAGS = -I$(IDIR) -lm -lpng -Wall

ODIR=./src/obj
LDIR =./lib
CDIR=./src

LIBS=

_DEPS = main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(CDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


$(DIR_OUTPUT)/main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 