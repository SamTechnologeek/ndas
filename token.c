/*
 * Copyright (c) 2014 Samuel Domínguez Lorenzo
 * <samuel.dominguez.lorenzo@gmail.com>
 * Released under the GPL v2
 */
#include "token.h"

/* note: we should get a string in the form 'contents\n\0' */
int tokenize(char *line, char **tokens)
{
	int tokcount = 0;
	int i;
	int lsize;
	
	if (line == NULL) return UNKNOWN;
	lsize = strlen(line);
	for (i = 0; i < lsize; ++i) {
		if (line[i] == ' ' || line[i] == '\t') continue;
		else break;
	}
	if (line[i] == ':') {
		/* it's a label.
		 * we should check for its validness.
		 * we could be tricked!
		 * *invalid* characters inside a label definition:
		 * ',' or ' ' or ':' or ';'
		 */
		/* TODO: check for label: label? LABEL : UNKNOWN */
		
		return LABEL;
	} else if (line[i] == '.') {
		/* it's a preprocessor directive.
		 * however we should check for its validness.
		 * we could be tricked like with the label!
		 */
		/* TODO: check for directive: directive? PREPR : UNKNOWN */
		return PREPR;
	} else if (line[i] == ';' || line[i] == '\n') {
		/* it's a comment or blank line.
		 * we shouldn't check, we are
		 * told to completely ignore
		 * stuff after comment symbols
		 * or do nothing with blank lines.
		 */
		return COMMWHITE;
	} else {
		/* alright. it's an instruction or we are
		 * getting completely trolled.
		 * so, we tokenize and check for the integrity of
		 * the data, if we see its not within our
		 * pattern list, we return UNKNOWN and recognize
		 * we have been trolled.
		 */
		/* TODO: INSTR? INSTR : UNKNOWN */
		return INSTR;
	}
}
	
void lowercase(char *s)
{
	int i;
	char c;
	if (s == NULL) return;

	for (i = 0; s[i]; ++i) {
		c = tolower(s[i]);
		s[i] = c;
	}
}
