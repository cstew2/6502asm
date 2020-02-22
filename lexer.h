#ifndef __LEXER_H__
#define __LEXER_H__

enum token_type{
	OPCODE,
	ADDRESS,
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
extern const struct token_alias TOKEN_ALIASES[14];

struct token {
	char *lexeme;
	enum token_type type;
};

struct token *lex(char *input);

#endif
