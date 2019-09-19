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

_OBJ = classifier.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(CDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


$(DIR_OUTPUT)/classifier: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 