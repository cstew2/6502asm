#ifndef __LEXER_H__
#define __LEXER_H__

#include "fileio.h"

enum token_type{
	OPCODE_TOK,
	REGISTER_TOK,
	DECIMAL_TOK,
	HEXIDECIMAL_TOK,
	BINARY_TOK,
	DIRECTIVE_TOK,
	LABEL_DEF_TOK,
	LABEL_REF_TOK,
	COMMENT_TOK,
	SEPERATOR_TOK,
	PAREN_L_TOK,
	PAREN_R_TOK,
	IMMEDIATE_TOK,
	EQUALS_TOK,
	STRING_TOK,
	TOKEN_END_TOK
};

struct token_alias {
	enum token_type type;
	char *name;
};

extern const int TOKEN_ALIAS_SIZE;
extern const struct token_alias TOKEN_ALIASES[15];

struct token {
	char *lexeme;
	enum token_type type;
};

struct token *lex(struct file *input);

struct token token_init(char *lexeme, enum token_type type);
int token_term(struct token t);

int token_list_term(struct token *t);

int is_special(char c);

#endif
