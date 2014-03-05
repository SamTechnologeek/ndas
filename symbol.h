#ifndef SYMBOL_H
#define SYMBOL_H

#include "ndas.h"

struct symbol {
	char *name;
	int value;
};

struct symtable {
	struct symbol data[MAX];
	int i;
};

struct symtable dsymbols;
struct symtable usymbols;

void add_usymbol(char *s, int val);
void add_dsymbol(char *s, int val);

#endif /* SYMBOL_H */
