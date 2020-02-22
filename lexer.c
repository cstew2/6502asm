#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "6502.h"

const int TOKEN_ALIAS_SIZE = 14;
const struct token_alias TOKEN_ALIASES[14] = {
	{OPCODE, "OPCODE"},
      	{ADDRESS, "ADDRESS"},
	{DECIMAL, "DECIMAL"},
	{HEXIDECIMAL, "HEXIDECIMAL"},
	{BINARY, "BINARY"},
	{DIRECTIVE, "DIRECTIVE"},
	{LABEL, "LABEL"},
	{COMMENT, "COMMENT"},
	{SEPERATOR, "SEPERATOR"},
	{PAREN_L, "PAREN_L"},
	{PAREN_R, "PAREN_R"},
	{EQUALS, "EQUALS"},
	{STRING, "STRING"},
	{TOKEN_END, "TOKEN_END"}
};

static const size_t INITIAL_SIZE = 128;
static const size_t MAX_LEXEME_SIZE = 256;

struct token *lex(char *input)
{

	struct token *tokens = malloc(sizeof(struct token) * INITIAL_SIZE);
	size_t size = INITIAL_SIZE;
	size_t token_count = 0;

	char lexeme[MAX_LEXEME_SIZE];
	enum token_type lexeme_type;
		
	size_t i=0;
	size_t j=0;
	while(input[i] != '\0') {
		if(isspace(input[i])) {
			while(isspace(input[i])) {
				i++;
			}
		}

		//string
		if(input[i] == '\"') {
			i++;
			while(input[i] != '\"') {
				lexeme[j++] = input[i++];
			}
			i++;
			lexeme[j] = '\0';
			lexeme_type = STRING;
		}
		//directive
		else if(input[i] == '.') {
			i++;
			//consume all alphanum until not
			while(!isspace(input[i])) {
				lexeme[j++] = input[i++];
			}
			lexeme[j] = '\0';
			//check if lexeme is a directive
			for(int k=0; k < DIRECTIVE_SIZE; k++) {
				if(!strcmp(lexeme, ASM_DIRECTIVES[k].string)) {
					lexeme_type = DIRECTIVE;
				}
			}
			i++;
		}
		//opcodes and labels
		else if(isalnum(input[i])) {
			//consume all alphanum until not
			while(!isspace(input[i])) {
				lexeme[j++] = input[i++];
			}
			lexeme[j] = '\0';
			
			//check if lexeme is an opcode
			for(int k=0; k < OPCODE_MNEMOIC_SIZE; k++) {
				if(!strcmp(lexeme, OPCODE_MNEMONICS[k])) {
					lexeme_type = OPCODE;
				}
			}
			
			//otherwise a label
			lexeme_type = LABEL;
		}
		//hexidecimal
		else if(input[i] == '$') {
			i++;
			while(isxdigit(input[i])) {
				lexeme[j++] = input[i++];	
			}
			lexeme[j] = '\0';
			lexeme_type = HEXIDECIMAL;
		}
		//decimal
		else if(input[i] == '#') {
			i++;
			while(isdigit(input[i])) {
				lexeme[j++] = input[i++];	
			}
			lexeme[j] = '\0';
			lexeme_type = DECIMAL;
		}
		//binary
		else if(input[i] == '%') {
			i++;
			while(isdigit(input[i])) {
				lexeme[j++] = input[i++];	
			}
			lexeme[j] = '\0';
			lexeme_type = BINARY;
		}
		//comment
		else if(input[i] == COMMENT_CHAR1 ||
			input[i] == COMMENT_CHAR2) {
			i++;
			while(input[i] != '\n') {
				lexeme[j++] = input[i++];
			}
			lexeme[j] = '\0';
			lexeme_type = COMMENT;
		}
		else {
			i++;
			//unknown token?
		}
		
		//add token to output
		if(token_count >= size) {
			tokens = realloc(tokens, sizeof(struct token) * size*2);
			size = size * 2;
		}
		
		printf("%s, %s\n", lexeme, TOKEN_ALIASES[lexeme_type].name);
		
		tokens[token_count].lexeme = malloc(sizeof(char) * strlen(lexeme)+1);
		strcpy(tokens[token_count].lexeme, lexeme);
		tokens[token_count++].type = lexeme_type;
		
		//next char and reset lexeme counter
		j=0;
		i++;
	}
	
	tokens = realloc(tokens, sizeof(struct token) * token_count+1);
	tokens[token_count].lexeme = "";
	tokens[token_count].type = TOKEN_END;
	return tokens;
}
