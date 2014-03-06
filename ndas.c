#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "ndas.h"
#include "opt.h"
#include "tokens.h"

int yylex(void);
extern FILE *yyin;

struct opt_t opts;
char defout[] = "a.bin";
union value lvalue;
int ndas_error = 0; /* if there is an error parsing the file, this is 1 */

static void display_usage(void)
{
	fprintf(stderr,
	"ndas - new dcpu-16 assembler; version %.2f\n", VERS);
	fprintf(stderr, "usage: ndas [options] [asmfiles]\n");
	fprintf(stderr, "options:\n");
	fprintf(stderr,
	"  -o outfile		write binary to outfile, default name is a.bin\n");
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

static void opts_free(void)
{
	opts.asm_fcount--;
	for (; opts.asm_fcount >= 0; opts.asm_fcount--) {
		if (opts.asm_fname[opts.asm_fcount] != NULL) {
			free(opts.asm_fname[opts.asm_fcount]);
			opts.asm_fname[opts.asm_fcount] = NULL;
		}
	}
	free(opts.asm_fname);
}

static void close_files(FILE **asmfiles, int n)
{
	for (--n; n >= 0; --n) {
		if (asmfiles[n]) fclose(asmfiles[n]);
	}
}

static void handle_args(int argc, char **argv)
{
	char opt = 0;
	int longind = 0;
	int outcount = 0;
	char **hdata = NULL;
	char *asm_file = NULL;
	static const char *optstring = "ho:";
	static const struct option longopts[] = {
		{"help", no_argument, NULL, 0},
		{"version", no_argument, NULL, 0},
	};

	opts.asm_fname = NULL;
	opts.bin_fname = NULL;
	opts.bin_name_spec = 0;
	opts.asm_fcount = 0;

	opt = getopt_long(argc, argv, optstring, longopts, &longind);

	while (opt != -1) {
		switch (opt) {
		case 'o':
			if (outcount == 0) {
				opts.bin_name_spec = 1;
				opts.bin_fname = optarg;
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

	if (optind == argc) {
		error("no input files specified");
		suggest_help();
		exit(EXIT_FAILURE);
	} else {
		while (optind < argc) {
			hdata = realloc(opts.asm_fname, sizeof (char*) * (opts.asm_fcount + 1));
			if (hdata == NULL) {
				error("memory error.");
				opts_free();
				exit(EXIT_FAILURE);
			}
			opts.asm_fname = hdata;
			opts.asm_fname[opts.asm_fcount] = malloc(sizeof (char) * strlen(argv[optind]) + 1);
			if (opts.asm_fname[opts.asm_fcount] == NULL) {
				error("memory error.");
				opts_free();
				exit(EXIT_FAILURE);
			}
			strcpy(opts.asm_fname[opts.asm_fcount], argv[optind]);
			opts.asm_fcount++;
			optind++;
		}
	}

	if (opts.bin_name_spec == 0) {
		opts.bin_fname = defout;
	}
}
 
int main(int argc, char **argv)
{
	char errstr[MAX];
	FILE* asmfiles[MAX];
	FILE *binfile = NULL;
	int lexval;
	int i;

	handle_args(argc, argv);

	for (i = 0; i < opts.asm_fcount; ++i) {
		printf("asm file[%d]: '%s'\n", i + 1, opts.asm_fname[i]);
		asmfiles[i] = fopen(opts.asm_fname[i], "r");
		if (asmfiles[i] == NULL) {
			sprintf(errstr, "couldn't open '%s'", opts.asm_fname[i]);
			error(errstr);
			error("perhaps it doesn't exist?");
			close_files(asmfiles, i);
			goto exit;
		}
	}
	if (opts.bin_name_spec) printf("bin file: '%s'\n", opts.bin_fname);
	else printf("bin file: '%s'\n", defout);

	for (i = 0; i < opts.asm_fcount; ++i) {
		yyin = asmfiles[i];
		while ((lexval = yylex()) != 0) {
			printf("token value: %d  value: %d\n", lexval, lvalue.integer);
		}
	}
	exit:
	close_files(asmfiles, opts.asm_fcount);
	opts_free();
	exit(EXIT_SUCCESS);
}
