#include <stdlib.h>
#include <stdio.h>

#include "asm.h"
#include "lexer.h"
#include "parser.h"
#include "fileio.h"

int assemble(char *input, char *output)
{
	//read in input
	struct file *input_file = NULL;
	input_file = read_file(input);
	
	//lexical analysis
       	struct token *tokens = NULL;
	tokens = lex(input_file);
	
	//semantic analysis
	
	
	//create binary
	//struct file *bin = file_init(i, );
		
	//write out output
	//write_file(output, bin);
	
        token_list_term(tokens);
	file_term(input_file);
	
       	return 0;
}

