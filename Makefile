CC=gcc
CFLAGS=-c -g
LDFLAGS=-lfl -ly -g
SOURCES=ndas.c output.c lex.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ndas

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

%.lex.c: %.l
	flex $<

clean:
	rm -fv lex.c
	rm -fv $(OBJECTS)
	rm -fv $(EXECUTABLE)
