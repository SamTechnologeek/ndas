/*
 * Copyright (c) 2014 Samuel Domínguez Lorenzo
 * <samuel.dominguez.lorenzo@gmail.com>
 * Released under the GPL v2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "opt.h"
#include "def.h"
#include "token.h"

struct opt_t opts;

static void display_usage(void) 
{
	fprintf(stderr, 
	"ndas - new dcpu-16 assembler, version %.2f\n", VERS);
	fprintf(stderr, "usage: ndas [options] asmfile\n");
	fprintf(stderr, "options:\n");
	fprintf(stderr, 
	"  -o outfile       write OF to outfile, default name is a.o\n");
	fprintf(stderr, 
	"  -I path          path for additional files\n");
	fprintf(stderr, 
	" --version         display version number and exit\n");
	fprintf(stderr, 
	" -h, --help        display this help and exit\n\n");
	exit(0);
}

inline static void display_version(void)
{
	fprintf(stdout, "ndas version: %.2f\n", VERS);
	exit(0);
}

inline static void suggest_help(void)
{
	fprintf(stderr, "ndas: type `ndas -h' for help\n");
}

inline static void error(char *s) 
{
	fprintf(stderr, "ndas: error: %s", s);
}

void static exit_ndas(void)
{
	if(opts.specout == FALSE) {
		if(opts.out != NULL) {
			free(opts.out);
			opts.out = NULL;
		}
	}
}

int main(int argc, char **argv)
{
	char opt = 0;
	int longind = 0;
	int pathcount = 0;
	int outcount = 0;
	char *defout = "a.obj";
	int i;
	FILE *asmfile = NULL;
	FILE *objfile = NULL;
	static const char *optstring = "hI:o:";
	static const struct option longopts[] = {
		{ "help", no_argument, NULL, 0 },
		{ "version", no_argument, NULL, 0},
	};
	
	int result;
	char teststring[] = "                .equ MAX 1337 ;comment\n";
	char teststring2[] = "\t:label\n";
	char teststring3[] = "\tJSR\tabs  ; for 32 shift\n";
	struct TOKENS tokens = { .data = NULL, .size = 0};
	char line[MAX];

	opts.filename = NULL;
	opts.path = NULL;
	opts.out = NULL;
	opts.specout = FALSE;

	opt = getopt_long(argc, argv, optstring, longopts, &longind);

	while(opt != -1) {
		switch(opt) {
		case 'o':
			if(outcount == 0) {
				opts.specout = TRUE;
				opts.out = optarg;
				outcount++;
			} else {
				error("output file specified multiple times\n");
				exit(1);
			}
			break;
		case 'I':
			if(pathcount == 0) {
				opts.path = optarg;
				pathcount++;
			} else {
				error("path specified multiple times\n");
				exit(1);
			}
			break;
		case 'h':
			display_usage();
			break;
		case '?':
			suggest_help();
			exit(1);
			break;
		case 0:
			if(strcmp("help", longopts[longind].name) == 0) {
				display_usage();
			} else if(strcmp("version", 
					longopts[longind].name) == 0) {
				display_version();
			}
			break;
		}
		opt = getopt_long(argc, argv, optstring, longopts, &longind);
	}

	if(optind < argc - 1) {
		error("too many input files specified\n");
		suggest_help();
		exit(1);
	} else if(optind == argc) {
		error("no input files specified\n");
		suggest_help();
		exit(1);
	} else {
		opts.filename = argv[optind];
	}

	if(opts.specout == FALSE) {
		opts.out = malloc(sizeof (char) * strlen(defout) + 1); 
		if(opts.out == NULL) {
			error("Out of memory\n");
			exit(1);
		}
		strcpy(opts.out, defout);
	}

	if(opts.filename != NULL) {
		printf("\nInput file: %s\n", opts.filename);
	}
	if(opts.out != NULL) {
		printf("Out file: %s\n", opts.out);
	}
	if(opts.path != NULL) {
		printf("Path : %s\n", opts.path);
	}

	asmfile = fopen(opts.filename, "r");

	if(asmfile == NULL) {
		error("specified input file doesn't exist\n");
		exit(1);
	}

	while (fgets(line, MAX, asmfile)) {
		result = tokenize(line, &tokens);
		printf("type: %d\n", result);
		printf("tokens n: %d\n", tokens.size);
		if (tokens.data != NULL) {
			printf("%p\n", tokens.data);
			for (i = 0; i < tokens.size; ++i) {
				printf(" %s ", tokens.data[i]);
			}
			printf("\n");
			for (i = 0; i < tokens.size; ++i) {
				if (tokens.data[i] != NULL) {
					printf("%p\n", tokens.data[i]);
					free(tokens.data[i]);
					tokens.data[i] = NULL;
				}
			}
			free(tokens.data);
			tokens.data = NULL;
			tokens.size = 0;	
		}
	}
	
	/*printf("line: '%s'\n", teststring3);
	result = tokenize(teststring3, &tokens);
	printf("statement type: %d\n", result);
	printf("tokens size: %d\n", tokens.size);
	if (tokens.data) {
		for (i = 0; i < tokens.size; ++i) {
			printf("token[%d]: '%s'\n", i, tokens.data[i]);
		}
		for (i = 0; i < tokens.size; ++i) {
			free(tokens.data[i]);
		}
		free(tokens.data);
	} else {
		printf("tokens.data is NULL.\n");
		return -1;
	}*/

	fclose(asmfile);	
	exit_ndas();
	return 0;
}
