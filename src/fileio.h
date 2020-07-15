#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <stddef.h>

struct file {
	size_t size;
	char *contents;
};

struct file *file_init(size_t size);
int file_term(struct file *f);

struct file *read_file(char *filename);
int write_file(char *filename, struct file *data);

#endif
