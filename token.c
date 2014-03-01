/*
 * Copyright (c) 2014 Samuel Domínguez Lorenzo
 * <samuel.dominguez.lorenzo@gmail.com>
 * Released under the GPL v2
 */
#include "token.h"

static void lowercase(char *s)
{
	int i;
	char c;
	if (s == NULL) return;

	for (i = 0; s[i]; ++i) {
		c = tolower(s[i]);
		s[i] = c;
	}
}

/* we support both kinds of label syntax.
 * 'label:' or ':label' it does not matter.
 * also we must check for validity.
 * non permitted characters are: ' ' and ',' and ':' and ';'
 */
static char *get_label(char *line, int size)
{
	static char label[MAX];
	int i, j;

	printf("inside get_label(...)\n");
	/* we get to the start of the label */	
	for (i = 0; i < size; ++i) {
		if (line[i] == ' ' || line[i] == '\t') continue;
		else break;
	}
	/* we get to the end of the label */
	for (j = i; j < size; ++j) {
		if (line[j] != ' ' && line[j] != '\t'
			&& line[j] != '\n' && line[j] != EOF)
			continue;
		else break;
	}
	--j;
	if (line[i] == ':' && line[j] != ':') {
		printf("given label is NOTCH like\n");
		/* it's notch label syntax */
		++i;
		++j;
		strncpy(label, line + i, j - i);
		/* the terminating null byte hasn't been placed */
		label[(j - i) + 1] = '\0';
		if (strchr(label, ' ') || strchr(label, ',') ||
			strchr(label, ':') || strchr(label, ';'))
			return NULL;
		else return label;
	} else if (line[i] != ':' && line[j] == ':') {
		printf("given label is NORMAL like\n");
		/* it's standard label syntax */
		strncpy(label, line + i, j - i);
		/* the terminating null byte hasn't been placed */
		label[(j - i) + 1] = '\0';
		if (strchr(label, ' ') || strchr(label, ',') ||
			strchr(label, ':') || strchr(label, ';'))
			return NULL;
		else return label;
	} else {
		printf("BAKA!\n");
		return NULL;
	}
}

/* note: we should get a string in the form 'contents\n\0' */
int tokenize(char *line, char ***tokens)
{
	int tokcount = 0;
	int i, j;
	int lsize;
	char *label;
	char *token;
	
	if (line == NULL) return UNKNOWN;
	lsize = strlen(line);
	for (i = 0; i < lsize; ++i) {
		if (line[i] == ' ' || line[i] == '\t') continue;
		else break;
	}
	for (j = i; j < lsize; ++j) {
		if (line[j] != ' ' && line[j] != '\t'
			&& line[j] != '\n') continue;
		else break;
	}
	--j;
	if (line[i] == '\n') {
		/* it's just blank */
		return COMMWHITE;
	} else if (line[i] == ':' || line[j] == ':') {
		/* it's a label.
		 * we should check for its validness.
		 * we could be tricked!
		 * *invalid* characters inside a label definition:
		 * ',' or ' ' or ':' or ';'
		 */
		label = get_label(line, lsize);	
		if (!label) return IN_LABEL;
		*tokens = malloc(sizeof (char*));
		if (!*tokens) {
			return MEM_ERROR;
		}
		*tokens[0] = malloc(sizeof (char) * strlen(label));
		if (!*tokens[0]) {
			free(*tokens);
			return MEM_ERROR;
		}
		strcpy(*tokens[0], label);
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
