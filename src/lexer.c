#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "6502.h"

const int TOKEN_ALIAS_SIZE = 15;
const struct token_alias TOKEN_ALIASES[15] = {
	{OPCODE_TOK,      "OPCODE"},
	{REGISTER_TOK,    "REGISTER"},
	{DECIMAL_TOK,     "DECIMAL"},
	{HEXIDECIMAL_TOK, "HEXIDECIMAL"},
	{BINARY_TOK,      "BINARY"},
	{DIRECTIVE_TOK,   "DIRECTIVE"},
	{LABEL_DEF_TOK,   "LABEL"},
	{LABEL_REF_TOK,   "LABEL"},
	{COMMENT_TOK,     "COMMENT"},
	{SEPERATOR_TOK,   "SEPERATOR"},
	{PAREN_L_TOK,     "PAREN_L"},
	{PAREN_R_TOK,     "PAREN_R"},
	{EQUALS_TOK,      "EQUALS"},
	{STRING_TOK,      "STRING"},
	{TOKEN_END_TOK,   "TOKEN_END"}
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
			lexeme_type = STRING_TOK;
		}
		//directive
		else if(input->contents[i] == DIRECTIVE_CHAR) {
			i++;
			//consume all alphanum until not
			while(!isspace(input->contents[i])) {
				lexeme[j++] = input->contents[i++];
			}
			lexeme[j] = '\0';
			//check if lexeme is a directive
			for(int k=0; k < DIRECTIVE_SIZE; k++) {
				if(!strcmp(lexeme, ASM_DIRECTIVES[k].string)) {
					lexeme_type = DIRECTIVE_TOK;
					break;
				}
			}
		}
		//opcodes, oprands and labels
		else if(isalpha(input->contents[i])) {
			//consume all chars until whitespace or special
			while(!isspace(input->contents[i]) &&
			      !is_special(input->contents[i])) {
				lexeme[j++] = input->contents[i++];
			}
			lexeme[j] = '\0';

			//label if ended with colon
			if(input->contents[i] == LABEL_CHAR) {
				lexeme[j+1] = '\0';
				lexeme_type = LABEL_DEF_TOK;
				i++;
			}
			else {
				lexeme_type = LABEL_REF_TOK;
			}
			
			//check if lexeme is an opcode
			for(int k=0; k < OPCODE_MNEMOIC_SIZE; k++) {
				if(!strcmp(lexeme, OPCODE_MNEMONICS[k])) {
					lexeme_type = OPCODE_TOK;
					break;
				}
			}

			//check if lexeme is a register
			for(int k=0; k < REGISTER_SIZE; k++) {
				if(!strcmp(lexeme, REGISTER_NAMES[k])) {
					lexeme_type = REGISTER_TOK;
					break;
				}
			}
		}
		//hexidecimal
		else if(input->contents[i] == HEX_CHAR) {
			i++;
			while(isxdigit(input->contents[i])) {
				lexeme[j++] = input->contents[i++];	
			}
			lexeme[j] = '\0';
			lexeme_type = HEXIDECIMAL_TOK;
		}
		//decimal
		else if(isdigit(input->contents[i])) {
			while(isdigit(input->contents[i])) {
				lexeme[j++] = input->contents[i++];	
			}
			lexeme[j] = '\0';
			lexeme_type = DECIMAL_TOK;
		}
		//binary
		else if(input->contents[i] == '%') {
			i++;
			while(isdigit(input->contents[i])) {
				lexeme[j++] = input->contents[i++];	
			}
			lexeme[j] = '\0';
			lexeme_type = BINARY_TOK;
		}
		//equals
		else if(input->contents[i] == EQUALS_CHAR) {
			lexeme[0] = input->contents[i];
			lexeme[1] = '\0';			
			lexeme_type = EQUALS_TOK;
			i++;
		}
		//immediate
		else if(input->contents[i] == IMMEDIATE_CHAR) {
			lexeme[0] = input->contents[i];
			lexeme[1] = '\0';			
			lexeme_type = IMMEDIATE_TOK;
			i++;
		}
		//seperator
		else if(input->contents[i] == SEPERATOR_CHAR) {
			lexeme[0] = input->contents[i];
			lexeme[1] = '\0';			
			lexeme_type = SEPERATOR_TOK;
			i++;
		}
		//left parenthesis
		else if(input->contents[i] == PAREN_L_CHAR) {
			lexeme[0] = input->contents[i];
			lexeme[1] = '\0';
			lexeme_type = PAREN_L_TOK;
			i++;
		}
		//right parenthesis
		else if(input->contents[i] == PAREN_R_CHAR) {
			lexeme[0] = input->contents[i];
			lexeme[1] = '\0';
			lexeme_type = PAREN_R_TOK;
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
			lexeme_type = COMMENT_TOK;
		}
		else {
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
	tokens[token_count].type = TOKEN_END_TOK;
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
	while(t[i].type != TOKEN_END_TOK) {
		token_term(t[i]);
		i++;
	}
	free(t);
	return 0;
}

int is_special(char c) {
	return (c == COMMENT_CHAR1  || c == COMMENT_CHAR2  || c == SEPERATOR_CHAR ||
		c == PAREN_L_CHAR   || c == PAREN_R_CHAR   || c == HEX_CHAR       ||
		c == BIN_CHAR       || c == IMMEDIATE_CHAR || c == LABEL_CHAR     ||
		c == DIRECTIVE_CHAR || c == EQUALS_CHAR);

}
