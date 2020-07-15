#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "6502.h"

const int TOKEN_ALIAS_SIZE = 13;
const struct token_alias TOKEN_ALIASES[13] = {
	{OPCODE, "OPCODE"},
	{REGISTER, "REGISTER"},
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

struct token *lex(struct file *input)
{

	struct token *tokens = calloc(sizeof(struct token), INITIAL_SIZE);
	size_t size = INITIAL_SIZE;
	size_t token_count = 0;

	char lexeme[MAX_LEXEME_SIZE];
	enum token_type lexeme_type;
		
	size_t i=0;
	size_t j=0;
	while(i < input->size) {
		if(isspace(input->contents[i])) {
			while(isspace(input->contents[i])) {
				i++;
			}
		}

		//string
		if(input->contents[i] == '\"') {
			i++;
			while(input->contents[i] != '\"') {
				lexeme[j++] = input->contents[i++];
			}
			i++;
			lexeme[j] = '\0';
			lexeme_type = STRING;
		}
		//directive
		else if(input->contents[i] == '.') {
			i++;
			//consume all alphanum until not
			while(!isspace(input->contents[i])) {
				lexeme[j++] = input->contents[i++];
			}
			lexeme[j] = '\0';
			//check if lexeme is a directive
			for(int k=0; k < DIRECTIVE_SIZE; k++) {
				if(!strcmp(lexeme, ASM_DIRECTIVES[k].string)) {
					lexeme_type = DIRECTIVE;
					break;
				}
			}
		}
		//opcodes, oprands and labels
		else if(isalnum(input->contents[i])) {
			//consume all chars until whitespace or special
			while(!isspace(input->contents[i]) &&
			      !is_special(input->contents[i])) {
				lexeme[j++] = input->contents[i++];
			}
			lexeme[j] = '\0';
			
			//check if lexeme is an opcode
			for(int k=0; k < OPCODE_MNEMOIC_SIZE; k++) {
				if(!strcmp(lexeme, OPCODE_MNEMONICS[k])) {
					lexeme_type = OPCODE;
					break;
				}
				
			}

			//check if lexeme is a register
			for(int k=0; k < REGISTER_SIZE; k++) {
				if(!strcmp(lexeme, REGISTER_NAMES[k])) {
					lexeme_type = REGISTER;
					break;
				}
				
			}
			
			//label if ended with colon
			if(input->contents[i] == ':') {
				lexeme[j+1] = '\0';
				lexeme_type = LABEL;
				i++;
			}
		}
		//hexidecimal
		else if(input->contents[i] == '$') {
			i++;
			while(isxdigit(input->contents[i])) {
				lexeme[j++] = input->contents[i++];	
			}
			lexeme[j] = '\0';
			lexeme_type = HEXIDECIMAL;
		}
		//decimal
		else if(input->contents[i] == '#') {
			i++;
			while(isdigit(input->contents[i])) {
				lexeme[j++] = input->contents[i++];	
			}
			lexeme[j] = '\0';
			lexeme_type = DECIMAL;
		}
		//binary
		else if(input->contents[i] == '%') {
			i++;
			while(isdigit(input->contents[i])) {
				lexeme[j++] = input->contents[i++];	
			}
			lexeme[j] = '\0';
			lexeme_type = BINARY;
		}
		//seperator
		else if(input->contents[i] == SEPERATOR_CHAR) {
			lexeme[0] = input->contents[i];
			lexeme[1] = '\0';			
			lexeme_type = SEPERATOR;
			i++;
		}
		//left parenthesis
		else if(input->contents[i] == PAREN_L_CHAR) {
			lexeme[0] = input->contents[i];
			lexeme[1] = '\0';
			lexeme_type = PAREN_L;
			i++;
		}
		//right parenthesis
		else if(input->contents[i] == PAREN_R_CHAR) {
			lexeme[0] = input->contents[i];
			lexeme[1] = '\0';
			lexeme_type = PAREN_R;
			i++;
		}
		//comments
		else if(input->contents[i] == COMMENT_CHAR1 ||
			input->contents[i] == COMMENT_CHAR2) {
			i++;
			while(input->contents[i] != '\n') {
				lexeme[j++] = input->contents[i++];
			}
			lexeme[j] = '\0';
			lexeme_type = COMMENT;
		}
		else {
			printf("\nerror: %c\n", input->contents[i]);
			i++;
			//unknown token?
		}
		
		//add token to output
		if(token_count >= size) {
			size = size * 2;
			tokens = realloc(tokens, sizeof(struct token) * size);
			
		}
		
		printf("%s - %s\n", lexeme, TOKEN_ALIASES[lexeme_type].name);
		
		tokens[token_count].lexeme = calloc(sizeof(char), strlen(lexeme)+1);
		strcpy(tokens[token_count].lexeme, lexeme);
		tokens[token_count].type = lexeme_type;
		token_count++;
		
		//next char and reset lexeme length counter
		j=0;
	}

       	tokens = realloc(tokens, sizeof(struct token) * (token_count+1));	
	tokens[token_count].lexeme = "";
	tokens[token_count].type = TOKEN_END;
	return tokens;
}

struct token token_init(char *lexeme, enum token_type type)
{
	struct token t;
	t.lexeme = lexeme;
	t.type = type;
	return t;
}

int token_term(struct token t)
{
	free(t.lexeme);
	return 0;
}

int token_list_term(struct token *t)
{
	size_t i = 0;
	while(t[i].type != TOKEN_END) {
		token_term(t[i]);
		i++;
	}
	free(t);
	return 0;
}

int is_special(char c) {
	return (c == COMMENT_CHAR1 || c == COMMENT_CHAR2  || c == SEPERATOR_CHAR ||
		c == PAREN_L_CHAR  || c == PAREN_R_CHAR   || c == HEX_CHAR       ||
		c == BIN_CHAR      || c == IMMEDIATE_CHAR || c == LABEL_CHAR     ||
		c == DIRECTIVE_CHAR);

}
