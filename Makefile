CC=gcc
CFLAGS=-c -g -Wno-packed-bitfield-compat
LDFLAGS=-lfl -ly -g
SOURCES=ndas.c output.c symbol.c lex.c yacc.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ndas

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

%.yacc.c: %.y
	bison -d $<

%.lex.c: %.l yacc.h
	flex $<

clean:
	rm -fv lex.c
	rm -fv yacc.c
	rm -fv yacc.h
	rm -fv $(OBJECTS)
	rm -fv $(EXECUTABLE)
