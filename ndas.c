#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "ndas.h"
#include "opt.h"

int yyparse(void);
int yylex(void);
extern FILE *yyin;

struct opt_t opts;
char defout[] = "a.o";
int ndas_error = 0; /* if there is an error parsing the file, this is 1 */

static void display_usage(void)
{
	fprintf(stderr,
	"ndas - new dcpu-16 assembler; version %.2f\n", VERS);
	fprintf(stderr, "usage: ndas [options] asmfile\n");
	fprintf(stderr, "options:\n");
	fprintf(stderr,
	"  -o outfile		write OF to outfile, default name is a.o\n");
	fprintf(stderr,
	"  --version		display version number and exit\n");
	fprintf(stderr,
	"  -h, --help		display this help and exit\n");
}

static void display_version(void)
{
	fprintf(stderr, "ndas version: %.2f\n", VERS);
}

static void suggest_help(void)
{
	fprintf(stderr, "ndas: type `ndas -h' for help\n");
}

static void error(char *s)
{
	fprintf(stderr, "ndas: error: %s\n", s);
}

static void handle_args(int argc, char **argv)
{
	char opt = 0;
	int longind = 0;
	int outcount = 0;
	static const char *optstring = "ho:";
	static const struct option longopts[] = {
		{"help", no_argument, NULL, 0},
		{"version", no_argument, NULL, 0},
	};

	opts.asm_fname = NULL;
	opts.obj_fname = NULL;
	opts.obj_name_spec = 0;

	opt = getopt_long(argc, argv, optstring, longopts, &longind);

	while (opt != -1) {
		switch (opt) {
		case 'o':
			if (outcount == 0) {
				opts.obj_name_spec = 1;
				opts.obj_fname = optarg;
				outcount++;
			} else {
				error("output file name already defined");
				exit(EXIT_FAILURE);
			}
			break;
		case 'h':
			display_usage();
			exit(EXIT_FAILURE);
			break;
		case '?':
			suggest_help();
			exit(EXIT_FAILURE);
		case 0:
			if (strcmp("help", longopts[longind].name) == 0) {
				display_usage();
				exit(EXIT_FAILURE);
			} else if (strcmp("version",
					longopts[longind].name) == 0) {
				display_version();
				exit(EXIT_FAILURE);
			}
			break;
		}
		opt = getopt_long(argc, argv, optstring, longopts, &longind);
	}

	if (optind < argc - 1) {
		error("too many input files specified");
		suggest_help();
		exit(EXIT_FAILURE);
	} else if (optind == argc) {
		error("no input files specified");
		suggest_help();
		exit(EXIT_FAILURE);
	} else {
		opts.asm_fname = argv[optind];
	}

	if (opts.obj_name_spec == 0) {
		opts.obj_fname = defout;
	}
}
 
int main(int argc, char **argv)
{
	FILE *asmfile = NULL;
	FILE *objfile = NULL;

	handle_args(argc, argv);

	if (opts.asm_fname) printf("asm file: '%s'\n", opts.asm_fname);
	if (opts.obj_name_spec) printf("obj file: '%s'\n", opts.obj_fname);
	else printf("obj file: '%s'\n", defout);

	asmfile = fopen(opts.asm_fname, "r");
	if (!asmfile) {
		error("couldn't open file, does it exist?");
		exit(EXIT_FAILURE);
	}
	yyin = asmfile;
	yylex();
	fclose(asmfile);
	exit(EXIT_SUCCESS);
}
