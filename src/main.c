#include <stdio.h>

#include "lexer.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "Please enter a file path");
        return 1;
    }

    FILE* stdinfp = freopen(argv[1], "r", stdin);
    if(stdinfp == NULL) {
        perror("error reading file");
        return 1;
    }

    FILE* stdoutfp = freopen("out.asm", "w", stdout);
    if(stdinfp == NULL) {
        perror("error piping stdout to out.asm");
        return 1;
    }

    fprintf(stderr, "running lexer test\n");

    lexer_progress_token();
    while(next_token != TOK_EOF) {

        fprintf(stderr, "%d : %s\n", next_token, lexer_token_to_str(next_token));

        int err = lexer_progress_token();
        if(err) break;
    }


    fclose(stdinfp);
    fclose(stdoutfp);

    return 0;
}
