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
int tokenize(char *line, struct TOKENS *tokens)
{
	int i = 0, j = 0;
	int lsize = 0;
	char *label = NULL;
	char exp[MAX];
	char *token = NULL;
	char **m_data = NULL;
	
	if (line == NULL) return UNKNOWN;
	tokens->size = 0;
	lowercase(line);
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
	if (line[i] == '\n' || line[i] == EOF) {
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
		tokens->data = malloc(sizeof (char*));
		if (!tokens->data) {
			return MEM_ERROR;
		}
		tokens->data[0] = malloc(sizeof (char) * strlen(label) + 1);
		if (!tokens->data[0]) {
			printf("free inside tokenize's label\n");
			free(tokens->data);
			tokens->data = NULL;
			return MEM_ERROR;
		}
		tokens->size = 1;	
		strcpy(tokens->data[0], label);
		return LABEL;
	} else if (line[i] == '.') {
		/* it's a preprocessor directive.
		 * however we should check for its validness.
		 * we could be tricked like with the label!
		 */
		
		for (j = i; j < lsize; ++j) {
			if (line[j] != '\n' && line[j] != EOF
				&& line[j] != ';') continue;
			else break;
		}
		strncpy(exp, line + i + 1, --j - i);
		exp[(j - i)] = '\0';

		tokens->data = malloc(sizeof (char *));
		if (!tokens->data) return MEM_ERROR;
		token = strtok(exp, PREP_DELIM);
		if (!token) {
			printf("What the hell. 1st token is NULL.\n");
			free(tokens->data);
			tokens->data = NULL;
			return UNKNOWN;
		}
		tokens->size = 1;
		tokens->data[0] = malloc(sizeof (char) * strlen(token));	
		strcpy(tokens->data[0], token);
		while ((token = strtok(NULL, PREP_DELIM)) != NULL) { 
			tokens->size++;
			printf("realloc size: %d\n", tokens->size * sizeof (char));
			m_data = realloc(tokens->data,
				tokens->size * sizeof (char*));
			if (!m_data) {
				printf("realloc() failed.\n");
				return UNKNOWN;
			}
			tokens->data = m_data;
			tokens->data[tokens->size - 1] =
					malloc(sizeof (char) * strlen(token));
			strcpy(tokens->data[tokens->size - 1], token);
		}

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
		
		for (j = i; j < lsize; ++j) {
			if (line[j] != '\n' && line[j] != EOF
				&& line[j] != ';') continue;
			else break;
		}
		strncpy(exp, line + i, j - i);
		exp[(j - i)] = '\0';

		tokens->data = malloc(sizeof (char *));
		if (!tokens->data) return MEM_ERROR;
		printf("exp: '%s'\n", exp);
		token = strtok(exp, INST_DELIM);
		printf("token: '%s'\n", token);
		if (!token) {
			printf("What the hell. 1st token is NULL.\n");
			free(tokens->data);
			tokens->data = NULL;
			return UNKNOWN;
		}
		tokens->size = 1;
		tokens->data[0] = malloc(sizeof (char) * strlen(token) + 1);	
		strcpy(tokens->data[0], token);
		while ((token = strtok(NULL, INST_DELIM)) != NULL) { 
			printf("token: '%s'\n", token);
			tokens->size++;
			printf("realloc size: %d\n", tokens->size * sizeof (char));
			m_data = realloc(tokens->data,
				tokens->size * sizeof (char*));
			if (!m_data) {
				printf("realloc() failed.\n");
				return UNKNOWN;
			}
			tokens->data = m_data;
			tokens->data[tokens->size - 1] =
				malloc(sizeof (char) * strlen(token) + 1);
			strcpy(tokens->data[tokens->size - 1], token);
		}

		return INSTR;
	}
}	
