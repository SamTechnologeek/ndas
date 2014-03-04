#include "symbol.h"

static struct symbol s;

struct symbol {
	char *name;
	int value;
};

struct symtable {
	struct symbol data[MAX];
	int i;
};

void add_dsymbol(char *s, int val)
{
	s.name = s;
	s.value = val;
	dsymbol.data[dsymbol.i++] = s;
}

void add_usymbol(char *s, int val)
{
	s.name = s;
	s.value = val;
	usymbol.data[usymbol.i++] = s;
}
