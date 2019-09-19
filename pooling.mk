DIR_OUTPUT = ./bin
IDIR =./include
CC = gcc
CFLAGS = -I$(IDIR)

ODIR=./src/obj
LDIR =./lib
CDIR=./src

LIBS=

_DEPS = main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = pooling.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(CDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


$(DIR_OUTPUT)/pooling: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 