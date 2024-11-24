CC = cc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`

SRC = sgwelcome.c
OBJ = $(SRC:.c=.o)
EXE = sgwelcome

all: $(EXE)
sgwelcome: sgwelcome.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
debug:
	$(CC) $(CFLAGS) -g $(SRC) -o debug $(LIBS)
test:
	./sgwelcome
clean:
	rm -f $(OBJ) $(EXE) debug
