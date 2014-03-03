#ifndef OPT_H
#define OPT_H

struct opt_t {
	char *asm_fname; /* name of the assembly file */
	char *obj_fname; /* name of the object file */
	int obj_name_spec; /* did the user specify an object name? */
};

#endif /* OPT_H */
