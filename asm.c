#include <stdlib.h>
#include <stdio.h>

#include "asm.h"
#include "lexer.h"
#include "parser.h"

int assemble(char *input, char *output)
{
	//read in input
	char *input_string = NULL;
	input_string = read_file(input);
	
	//lexical analysis
       	struct token *tokens = NULL;
	tokens = lex(input_string);
	
	//semantic analysis
	size_t i = 0;
	while(tokens[i].type != TOKEN_END) {
		//printf("%s, %s\n", tokens[i].lexeme, TOKEN_ALIASES[tokens[i].type].name);
		i++;
	}
	i++;
	//printf("%s, %s\n", tokens[i].lexeme, TOKEN_ALIASES[tokens[i].type].name);
	
	//create binary
	uint8_t *binary = NULL;
	binary = malloc(sizeof(uint8_t) * 0xFF);
	
	//write out output
	//write_file(output, binary);

	free(tokens);
	free(binary);
	
       	return 0;
}

char *read_file(char *filename)
{
	FILE *f = NULL;
	size_t size = 0;
	f = fopen(filename, "r");

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *data = malloc(sizeof(char) * size);
	fread(data, sizeof(char), size, f);
	fclose(f);
	return data;
}

int write_file(char *filename, uint8_t *data)
{
	FILE *f = NULL;
	f = fopen(filename, "wb");
	fwrite(data, sizeof(uint8_t), 0xFF, f);
	fclose(f);
	return 0;
}
