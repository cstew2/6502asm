#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileio.h"

struct file *file_init(size_t size)
{
	struct file *f = calloc(sizeof(struct file), 1);
	f->size = size;
	f->contents = calloc(sizeof(char), size);
	return f;
}

int file_term(struct file *f)
{
	free(f->contents);
	free(f);
}

struct file *read_file(char *filename)
{
	FILE *f = NULL;
	size_t size = 0;
	f = fopen(filename, "r");

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	struct file *r = file_init(size);
	fread(r->contents, sizeof(char), size, f);
	
	fclose(f);
	return r;
}

int write_file(char *filename, struct file *data)
{
	FILE *f = NULL;
	f = fopen(filename, "wb");
	fwrite(data, sizeof(char), 0xFF, f);
	fclose(f);
	return 0;
}
