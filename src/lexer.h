#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>


#define MAX_IDENT_STRING 200

enum Token {
    // misc
    TOK_EMPTY = 0,
    TOK_EOF = -1,

    // symbols
    TOK_LPARENTHESES = '(',
    TOK_RPARENTHESES = ')',
    TOK_LBRACES = '{',
    TOK_RBRACES = '}',
    TOK_LSQUAREBRACKET = '[',
    TOK_RSQUAREBRACKET = ']',
    TOK_SEMICOLON = ';',

    // asignment
    TOK_ASSIGN = '=',

    // math
    TOK_ADD = '+',
    TOK_SUB = '-',
    TOK_DIV = '/',
    TOK_MOD = '%',
    TOK_MUL = '*',

    // values
    TOK_IDENT = 130,
    TOK_NUMBER,

    // boolean operators
    TOK_EQ,
    TOK_NEQ,
    TOK_LT,
    TOK_LTE,
    TOK_GT,
    TOK_GTE,

    // datatypes
    TOK_INT,

    // commands
    TOK_RETURN,


};


extern enum Token next_token;
extern int32_t next_token_value;
extern char next_ident_string[MAX_IDENT_STRING];

extern int current_line;
extern int current_col;


int lexer_progress_token();
const char* lexer_token_to_str(enum Token tok);

#endif
