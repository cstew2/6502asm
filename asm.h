#ifndef __ASM_H__
#define __ASM_H__

#include <stdint.h>

int assemble(char *input, char *output);
char *read_file(char *filename);
int write_file(char *filename, uint8_t *data);
	
#endif
