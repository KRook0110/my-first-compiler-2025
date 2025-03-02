#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

enum Token next_token;
int32_t next_token_value;
char next_ident_string[MAX_IDENT_STRING];

int current_line = 0;
int current_col = 0;

char lexer_progress_char() {
    char c = getchar();
    if (c == '\n') {
        current_line++;
    }
    current_col++;
    return c;
}

int lex_alnum(char* dest, int max_length, char* next_char) {
    int current_size = 0;
    while (isalnum(*next_char)) {
        // leaves one space for \0
        if (current_size + 1 >= max_length) {
            fprintf(stderr, "lexer_alnum : max_length is too small\n");
            return 1;
        }

        dest[current_size] = *next_char;
        current_size++;
        *next_char = lexer_progress_char();
    }
    dest[current_size] = '\0';
    return 0;
}

int lex_number(char* next_char, int32_t* target) {
    *target = 0;
    while (isdigit(*next_char)) {
        if (*target > INT_MAX / 10) {
            fprintf(stderr, "lexer_progress_token : Int literal to big");
            return 1;
        }
        *target *= 10;
        if (*next_char - '0' > INT_MAX - *target) {
            fprintf(stderr, "lexer_progress_token : Int literal to big");
            return 1;
        }
        *target += *next_char - '0';
        *next_char = lexer_progress_char();
    }
    return 0;
}

// return
// 1 -> not found
// 0 -> found
int lex_symbols(char* next_char) {
    switch (*next_char) {
        case '{':
        case '}':
        case '(':
        case ')':
        case '[':
        case ']':
        case ';':
        case '+':
        case '-':
        case '/':
        case '*':
        case '%':
            next_token = *next_char;
            *next_char = lexer_progress_char();
            return 0;
        case '=':
            *next_char = lexer_progress_char();

            // "=="
            if (*next_char == '=') {
                next_token = TOK_EQ;
                *next_char = lexer_progress_char();
                return 0;
            }

            next_token = TOK_ASSIGN;
            return 0;
        case '<':
            *next_char = lexer_progress_char();
            // "<="
            if (*next_char == '=') {
                next_token = TOK_LTE;
                *next_char = lexer_progress_char();
                return 0;
            }

            next_token = TOK_LT;
            return 0;
        case '>':
            *next_char = lexer_progress_char();
            // ">="
            if (*next_char == '=') {
                next_token = TOK_GTE;
                *next_char = lexer_progress_char();
                return 0;
            }
            next_token = TOK_GT;
            return 0;
    }
    return 1;
}

// progresses the lexer token
int lexer_progress_token() {
    static char next_char = ' ';
    // remove the whitespace
    while (isspace(next_char) || next_char == '\n') {
        next_char = lexer_progress_char();
    }

    if (next_char == EOF) {
        next_token = TOK_EOF;
        return 0;
    };

    if (isalpha(next_char)) {
        int err = lex_alnum(next_ident_string, MAX_IDENT_STRING, &next_char);
        if (err) {
            fprintf(stderr, "lexer_progress_token : Error on lexer_alnum\n");
            return 1;
        }

        if (strcmp(next_ident_string, "return") == 0) {
            next_token = TOK_RETURN;
            return 0;
        }
        if (strcmp(next_ident_string, "int") == 0) {
            next_token = TOK_INT;
            return 0;
        }

        next_token = TOK_IDENT;
        return 0;
    }
    if (isdigit(next_char)) {
        int err = lex_number(&next_char, &next_token_value);
        if (err) {
            fprintf(stderr, "lexer_progress_token : Error on lexer_number\n");
            return 1;
        }
        next_token = TOK_NUMBER;
        return 0;
    }
    int notfound = lex_symbols(&next_char);
    if (!notfound) return 0;

    fprintf(stderr, "Token is unidentified %c : %d\n", next_char, next_char);
    return 1;
}

const char* lexer_token_to_str(enum Token tok) {
    switch (tok) {
        case TOK_EMPTY:
            return "TOK_EMPTY";
        case TOK_LPARENTHESES:
            return "TOK_LPARENTHESES";
        case TOK_RPARENTHESES:
            return "TOK_RPARENTHESES";
        case TOK_LBRACES:
            return "TOK_LBRACES";
        case TOK_RBRACES:
            return "TOK_RBRACES";
        case TOK_LSQUAREBRACKET:
            return "TOK_LSQUAREBRACKET";
        case TOK_RSQUAREBRACKET:
            return "TOK_RSQUAREBRACKET";
        case TOK_ADD:
            return "TOK_ADD";
        case TOK_SUB:
            return "TOK_SUB";
        case TOK_DIV:
            return "TOK_DIV";
        case TOK_MOD:
            return "TOK_MOD";
        case TOK_MUL:
            return "TOK_MUL";
        case TOK_IDENT:
            return "TOK_IDENT";
        case TOK_NUMBER:
            return "TOK_NUMBER";
        case TOK_INT:
            return "TOK_INT";
        case TOK_RETURN:
            return "TOK_RETURN";
        case TOK_EOF:
            return "TOK_EOF";
        case TOK_ASSIGN:
            return "TOK_ASSIGN";
        case TOK_EQ:
            return "TOK_EQ";
        case TOK_NEQ:
            return "TOK_NEQ";
        case TOK_LT:
            return "TOK_LT";
        case TOK_LTE:
            return "TOK_LTE";
        case TOK_GT:
            return "TOK_GT";
        case TOK_GTE:
            return "TOK_GTE";
        case TOK_SEMICOLON:
            return "TOK_SEMICOLON";
    }
    return "Unidentified token";
}
