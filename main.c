#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"

#include "asm.h"

int main(int argc, char **argv)
{
	char *input = NULL;
	char *output = NULL;
	bool d_flag = false;
	bool v_flag = false;
	
	for(int i=1; i < argc; i++) {
		if(!strcmp(argv[i], "-d")) {
			d_flag = true;
		}
		if(!strcmp(argv[i], "-v")) {
			v_flag = true;
		}
		else if(!strcmp(argv[i], "-f")) {
		       i++;
		       input = malloc(sizeof(char) * strlen(argv[i])+1);
		       strcpy(input, argv[i]);
		}
		else if(!strcmp(argv[i], "-o")) {
		       i++;
		       output = malloc(sizeof(char) * strlen(argv[i])+1);
		       strcpy(output, argv[i]);
		}
		else if(!strcmp(argv[i], "--help") ||
			!strcmp(argv[i], "-h")) {
			print_help();
		}
	}

	if(d_flag) {
		
	}
	if(v_flag) {
		
	}

	if(input == NULL) {
		printf("require a input file\n");
		return -1;
	}

	if(input == NULL) {
		output = "outfile.bin";
	}
	
	
	//run assembler
	assemble(input, output);	

	
	//clean up
	if(input != NULL) {
		free(input);
	}
	if(output != NULL) {
		free(output);
	}
	
	return 0;
}

void print_help(void)
{
	printf("6502asm usage: 6502asm [OPTIONS]..\n\n"
	       "OPTIONS\n"
	       "-d \t debug information"
	       "-v \t verbose output"
	       "-f \t input file\n");
}
