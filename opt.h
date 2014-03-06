#ifndef OPT_H
#define OPT_H

struct opt_t {
	char **asm_fname; /* name of the assembly file */
	char *bin_fname; /* name of the object file */
	int bin_name_spec; /* did the user specify an object name? */
	int asm_fcount; /* how many asm files? */
};

#endif /* OPT_H */
