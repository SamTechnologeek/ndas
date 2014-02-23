/*
 * Copyright (c) 2014 Samuel Domínguez Lorenzo
 * <samuel.dominguez.lorenzo@gmail.com>
 * Released under the GPL v2
 */
#ifndef OPT_H
#define OPT_H

struct opt_t {
	char *filename; /* Name if the file to assemble */
	char *path; /* Path to look for, apart from current directory */
	char *out; /* Output file name, default is a.obj */
	int specout; /* Did the user specify an output filename ? */
};

#endif /* opt.h */
