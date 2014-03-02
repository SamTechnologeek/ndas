/*
 * Copyright (c) 2014 Samuel Domínguez Lorenzo
 * <samuel.dominguez.lorenzo@gmail.com>
 * Released under the GPL v2
 */
#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "def.h"

/* different types of token and other possible encounters */
#define UNKNOWN 	0x0000 /* unknown, we exit */
#define INSTR       	0x0001 /* an instruction, e.g. SET A, 2, JSR abs */
#define LABEL       	0x0002 /* a valid label */
#define PREPR       	0x0003 /* a valid preprocessor directive */
#define COMMWHITE   	0x0004 /* a valid comment or whitespace */

/* different errors after checking for validness, note IN stands for invalid */
#define IN_LABEL    	0x0005
#define IN_PREPR    	0x0006
#define IN_INSTR    	0x0007
#define MEM_ERROR	0x0008

/* delimiters for splitting the string */
#define INST_DELIM    	" ,\t" /* either a space, a comma or a tab */
#define PREP_DELIM	" \t"

struct TOKENS {
	char **data;
	int size;
};

/*
 * takes a *line of the file and splits it to get the opcode and arguments
 * and returns them into **tokens. The type of line is returned, and
 * **tokens will be filled only if its a instruction, the linker will care
 * about LABELS and PREPROCESSOR DIRECTIVES.
 */
int tokenize(char *line, struct TOKENS *tokens);

#endif /* TOKEN_H */
