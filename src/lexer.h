#ifndef __LEXER_H__
#define __LEXER_H__

#include "fileio.h"

enum token_type{
	OPCODE,
	REGISTER,
	DECIMAL,
	HEXIDECIMAL,
	BINARY,
	DIRECTIVE,
	LABEL,
	COMMENT,
	SEPERATOR,
	PAREN_L,
	PAREN_R,
	EQUALS,
	STRING,
	TOKEN_END
};

struct token_alias {
	enum token_type type;
	char *name;
};

extern const int TOKEN_ALIAS_SIZE;
extern const struct token_alias TOKEN_ALIASES[13];

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
