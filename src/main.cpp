#include <stdlib.h>
#include <string.h>

#include <cstdlib>
#include <iostream>

#define MAX_IDENT_CHAR 100

enum Token {
    NO_TOKEN = 0,
    TOK_EOF = -1,
    TOK_RETURN = -2,

    // Data types
    TOK_INTEGER = -3,

    //  Values
    TOK_NUMBER = -4,

    // expressions
    TOK_ADD = '+',
    TOK_SUB = '-',
    TOK_MULT = '*',
    TOK_DIV = '/',
    TOK_MOD = '%',

    // other symbols
    TOK_LCBRACKET = '{',
    TOK_RCBRACKET = '}',
    TOK_LPAREN = '(',
    TOK_RPAREN = ')',
    TOK_SEMICOLON = ';',
    TOK_ASSIGN = '=',

    // misc
    TOK_IDENT = -5,
    TOK_START = -6,
};

void print_token(Token tok) {
    switch (tok) {
        case NO_TOKEN:
            std::cerr << "NO_TOKEN\n";
            break;
        case TOK_EOF:
            std::cerr << "TOK_EOF\n";
            break;
        case TOK_RETURN:
            std::cerr << "TOK_RETURN\n";
            break;
        case TOK_INTEGER:
            std::cerr << "TOK_INTEGER\n";
            break;
        case TOK_NUMBER:
            std::cerr << "TOK_NUMBER\n";
            break;
        case TOK_ADD:
            std::cerr << "TOK_ADD\n";
            break;
        case TOK_SUB:
            std::cerr << "TOK_SUB\n";
            break;
        case TOK_MULT:
            std::cerr << "TOK_MULT\n";
            break;
        case TOK_DIV:
            std::cerr << "TOK_DIV\n";
            break;
        case TOK_MOD:
            std::cerr << "TOK_MOD\n";
            break;
        case TOK_LCBRACKET:
            std::cerr << "TOK_LCBRACKET\n";
            break;
        case TOK_RCBRACKET:
            std::cerr << "TOK_RCBRACKET\n";
            break;
        case TOK_LPAREN:
            std::cerr << "TOK_LPAREN\n";
            break;
        case TOK_RPAREN:
            std::cerr << "TOK_RPAREN\n";
            break;
        case TOK_IDENT:
            std::cerr << "TOK_IDENT\n";
            break;
        case TOK_SEMICOLON:
            std::cerr << "TOK_SEMICOLON\n";
            break;
        default:
            std::cerr << "IDK What is this\n";
            break;
    }
}


struct Symbol {
    Token token;
    std::string idname;
    int idvalue;
};

Symbol next_symbol;

void progress_symbol() {
    static char curchar = getchar();
    while (isspace(curchar) && curchar != EOF) {
        curchar = getchar();
    }
    if (curchar == EOF) {
        next_symbol.token = TOK_EOF;
        return;
    }
    if (isalpha(curchar)) {
        std::string t;
        while (isalnum(curchar)) {
            t += curchar;
            curchar = getchar();
        }
        if (t == "int") {
            next_symbol.token = TOK_INTEGER;
            return;
        }
        if (t == "return") {
            next_symbol.token = TOK_RETURN;
            return;
        }
        next_symbol.idname = std::move(t);
        next_symbol.token = TOK_IDENT;
        return;
    }

    if (isdigit(curchar)) {
        int t = 0;
        while (isdigit(curchar)) {  // doesn't handles float
            t *= 10;
            t += (curchar - '0');
            curchar = getchar();
        }
        next_symbol.token = TOK_NUMBER;
        next_symbol.idvalue = t;
        return;
    }

    switch (curchar) {
        case '{':
        case '}':
        case '(':
        case ')':
        case '+':
        case '-':
        case '/':
        case '*':
        case '%':
        case ';':
            next_symbol.token = (Token)curchar;
            curchar = getchar();
            return;
        default:
            break;
    }
    // token failed to find token
    std::cerr << "Lexer failed to find token" << '\n';
    exit(1);
    return;
}

bool accept(Token tok) {
    if(tok == next_symbol.token) {
        return true;
    }
    return false;
}
bool expect(Token tok) {
    if(accept(tok)) {
        return true;
    }
    std::cerr << "expected ";
    print_token(tok);
    std::cerr << "\n";
    return false;
}


// parser
enum NodeType {
    NODE_START,
    NODE_NUMBER,
    NODE_BINARY_OP,
    NODE_RETURN,
    NODE_FUNCTION,
    NODE_BLOCK,
    NODE_NEG,
    NODE_VARIABLE,
};

struct ASTNode {
    NodeType type;
    union {
        // NUMBER
        int number;
        // BINARY OP
        struct {
            ASTNode* left;
            char op;
            ASTNode* right;
        };
        // NODE_RETURN, NODE_NEG
        struct {
            ASTNode* expression;  // for now only supports numbers
        };
        // NODE_FUNCTION
        struct {
            char* function_name;
            ASTNode* function_body;
        };
        // NODE_VARIABLE
        struct {
            char* variable_name;
            Token variable_datatype;
            ASTNode*
                variable_assignment;  // for assignment, expects an expresion,
                                      // left NULL if there is not assignment;
        };
        // NODE_BLOCK
        struct {
            ASTNode** statements;
            int statements_count;
        };
        // NODE_START
        struct {
            ASTNode** globals;
            int globals_count;
        };
    };
};

ASTNode* parse_expression();

ASTNode* parse_number() {
    if (next_symbol.token != TOK_NUMBER) {
        std::cerr << "expected number\n";
        return NULL;
    }
    ASTNode* number_node = (ASTNode*)malloc(sizeof(ASTNode));
    number_node->type = NODE_NUMBER;
    number_node->number = next_symbol.idvalue;
    progress_symbol();
    return number_node;
}

int precedence[130] = {};

ASTNode* parse_simple() {
    if (next_symbol.token == TOK_SUB) {
        progress_symbol();
        ASTNode* neg_node = parse_simple();
        neg_node->type = NODE_NEG;
        neg_node->expression = parse_simple();
        return neg_node;
    }
    if (next_symbol.token == TOK_NUMBER) {
        return parse_number();
    }
    if (next_symbol.token == TOK_LPAREN) {
        progress_symbol();

        ASTNode* t = parse_expression();

        if (next_symbol.token != TOK_RPAREN) {
            std::cout << "expected ')'\n" << '\n';
            return t;  // just ignore and still try to parse
        }

        progress_symbol();
        return t;
    }
    std::cout << "expected simple number, or '('\n";
    return NULL;
}

ASTNode* parse_expression_rec(ASTNode* lhs, int cur_precedence) {
    if (lhs == NULL) {
        lhs = parse_simple();
        if (lhs == NULL) {
            std::cout << "expected simple form\n";
            return NULL;
        }
    }
    while (cur_precedence <= precedence[next_symbol.token] &&
           precedence[next_symbol.token] != 0) {
        if (cur_precedence <
            precedence[next_symbol.token]) {  // if next precedence is higher
            if (lhs->type == NODE_NUMBER)
                lhs = parse_expression_rec(lhs, cur_precedence + 1);
            else
                lhs->right =
                    parse_expression_rec(lhs->right, cur_precedence + 1);
            continue;
        }
        // if the precedence is the same
        ASTNode* new_head = (ASTNode*)malloc(sizeof(ASTNode));
        new_head->type = NODE_BINARY_OP;
        new_head->left = lhs;
        new_head->op = next_symbol.token;
        progress_symbol();
        new_head->right = parse_simple();
        lhs = new_head;
    }
    return lhs;
}

ASTNode* parse_expression() { return parse_expression_rec(NULL, 0); }

ASTNode* parse_statement() {
    ASTNode* statement_node = (ASTNode*)malloc(sizeof(ASTNode));
    if (next_symbol.token == TOK_RETURN) {
        progress_symbol();
        statement_node->type = NODE_RETURN;
        statement_node->expression = parse_expression();

        if (next_symbol.token != TOK_SEMICOLON) {
            std::cerr << "expected ';'\n";
        }

        progress_symbol();
        return statement_node;
    }
    return NULL;
}

ASTNode* parse_block() {
    ASTNode* block_node = (ASTNode*)malloc(sizeof(ASTNode));
    block_node->type = NODE_BLOCK;
    block_node->statements = NULL;
    block_node->statements_count = 0;
    ASTNode* statement;
    do {
        statement = parse_statement();
        if (statement == NULL) break;
        block_node->statements_count++;
        block_node->statements =
            (ASTNode**)realloc(block_node->statements,
                               sizeof(ASTNode*) * block_node->statements_count);
        block_node->statements[block_node->statements_count - 1] = statement;
    } while (1);

    if (block_node->statements_count == 0) {
        std::cerr << "expected statement on block\n";
        return NULL;
    }
    return block_node;
}

void print_token(Token tok);

ASTNode* parse_declaration() {
    Token datatype;
    switch (next_symbol.token) {
        case TOK_INTEGER:  // add more datatypes below here
            datatype = next_symbol.token;
            break;
        default:
            std::cerr << "expected datatype\n";
            return NULL;
    }
    progress_symbol();

    if(!expect(TOK_IDENT)) return NULL;
    std::string name = next_symbol.idname;
    progress_symbol();

    // variable
    if (next_symbol.token == TOK_SEMICOLON || next_symbol.token == TOK_ASSIGN) {
        bool is_assigned = next_symbol.token == TOK_ASSIGN;
        progress_symbol();

        ASTNode* variable_node = (ASTNode*)malloc(sizeof(ASTNode));
        variable_node->type = NODE_VARIABLE;
        strcpy(variable_node->variable_name, name.c_str());
        variable_node->variable_datatype = datatype;
        if (is_assigned) {
            variable_node->variable_assignment = parse_expression();
            if(next_symbol.token != TOK_SEMICOLON) {
                std::cerr << "Expected ';'\n";
                return NULL;
            }
            progress_symbol();
            variable_node->variable_assignment = parse_expression();
        } else {
            variable_node->variable_assignment = NULL;
        }

        return variable_node;
    }
    //function
    if (accept(TOK_LPAREN)) {      // then this is a function call;

        progress_symbol();
        if(!expect(TOK_RPAREN)) return NULL;
        progress_symbol();
        if(!expect(TOK_LCBRACKET)) return NULL;
        progress_symbol();

        ASTNode* function_node = (ASTNode*)malloc(sizeof(ASTNode*));
        function_node->type = NODE_FUNCTION;
        strcpy(function_node->function_name, name.c_str());
        function_node->function_body = parse_block();

        if(!expect(TOK_RCBRACKET)) return NULL;
        progress_symbol();
    }

    std::cerr << "expected '(', '=', or ';'\n";
    return NULL;
}

ASTNode* parse_function() {
    ASTNode* function_node = (ASTNode*)malloc(sizeof(ASTNode));
    function_node->type = NODE_FUNCTION;

    if (next_symbol.token != TOK_INTEGER) {  // int
        std::cerr << "expected Integer Token\n";
        return NULL;
    }
    progress_symbol();

    if (next_symbol.token != TOK_IDENT) {  // function name
        std::cerr << "expected Identifier Token\n";
        return NULL;
    }
    function_node->function_name = (char*)malloc(sizeof(char) * MAX_IDENT_CHAR);
    strcpy(function_node->function_name, next_symbol.idname.c_str());
    progress_symbol();

    if (next_symbol.token != TOK_LPAREN) {  // (
        std::cerr << "expected '('\n";
        return NULL;
    }
    progress_symbol();

    if (next_symbol.token != TOK_RPAREN) {  // )
        std::cerr << "expected ')'\n";
        return NULL;
    }
    progress_symbol();
    if (next_symbol.token != TOK_LCBRACKET) {  // {
        std::cerr << "expected '{'\n";
        return NULL;
    }

    progress_symbol();

    // body
    function_node->function_body = parse_block();

    if (function_node->function_body == NULL) {
        std::cerr << "function is empty\n";
    }

    if (next_symbol.token != TOK_RCBRACKET) {  // }
        std::cerr << "expected '}'\n";
        return NULL;
    }
    progress_symbol();

    return function_node;
}

ASTNode* parse_start() {
    ASTNode* node_start = (ASTNode*)malloc(sizeof(ASTNode));
    node_start->type = NODE_START;
    node_start->globals = NULL;
    node_start->globals_count = 0;

    while (next_symbol.token == TOK_INTEGER) {
        node_start->globals_count++;
        node_start->globals = (ASTNode**)realloc(
            node_start->globals, sizeof(ASTNode*) * node_start->globals_count);
        node_start->globals[node_start->globals_count - 1] = parse_declaration();
    };

    if (next_symbol.token != TOK_EOF) {
        std::cerr << "expected function\n";
    }
    return node_start;
}

// end parsing


std::string tostring_node(NodeType type) {
    switch (type) {
        case NODE_START:
            return "NODE_START";
        case NODE_NUMBER:
            return "NODE_NUMBER";
        case NODE_BINARY_OP:
            return "NODE_BINARY_OP";
        case NODE_RETURN:
            return "NODE_RETURN";
        case NODE_FUNCTION:
            return "NODE_FUNCTION";
        case NODE_BLOCK:
            return "NODE_BLOCK";
        case NODE_NEG:
            return "NODE_NEG";
        case NODE_VARIABLE:
            return "NODE_VARIABLE";
          break;
        }
    return "idk";
}

#define SPACES_DIFF_AST_PRINT 2
void print_AST(ASTNode* node, int spaces) {
    for (int i = 0; i < spaces; i++) std::cerr << ' ';
    std::cerr << tostring_node(node->type) << " begin >" << '\n';
    int next_spaces = spaces + SPACES_DIFF_AST_PRINT;
    switch (node->type) {
        case NODE_START:
            for (int i = 0; i < node->globals_count; i++) {
                print_AST(node->globals[i], next_spaces);
            }
            break;
        case NODE_NUMBER:
            for (int i = 0; i < spaces; i++) std::cerr << ' ';
            std::cerr << "Value : " << node->number << '\n';
            break;
        case NODE_BINARY_OP:  // not implemented yet
            print_AST(node->left, next_spaces);
            for (int i = 0; i < spaces; i++) std::cerr << ' ';
            std::cerr << "op : " << node->op << '\n';
            print_AST(node->right, next_spaces);
            break;
        case NODE_RETURN:
            print_AST(node->expression, next_spaces);
            break;
        case NODE_FUNCTION:
            for (int i = 0; i < spaces; i++) std::cerr << ' ';
            std::cerr << "function name : " << node->function_name << '\n';
            print_AST(node->function_body, next_spaces);
            break;
        case NODE_BLOCK:
            for (int i = 0; i < node->statements_count; i++) {
                print_AST(node->statements[i], next_spaces);
            }
            break;
        case NODE_NEG:
            print_AST(node->expression, next_spaces);
            break;
        case NODE_VARIABLE:
            for (int i = 0; i < spaces; i++) std::cerr << ' ';
            std::cerr << "variable name : " << node->variable_name << '\n';
            print_AST(node->variable_assignment, next_spaces);
            break;
    }
    for (int i = 0; i < spaces; i++) std::cerr << ' ';
    std::cerr << tostring_node(node->type) << " end >" << '\n';
}

// Code Generation
void generate_code(ASTNode* node) {
    switch (node->type) {
        case NODE_START:
            std::cout << "section .text\n";

            std::cout << "\tglobal _start\n";
            std::cout << "_start:\n";
            std::cout << "\tcall main\n";
            std::cout << "\tmov rdi, rax\n";
            std::cout << "\tmov rax, 60\n";
            std::cout << "\tsyscall\n";

            for (int i = 0; i < node->globals_count; i++) {
                generate_code(node->globals[i]);
            }
            break;

        case NODE_NUMBER:  // tbh I don't know how to handle this
            std::cout << "\tmov rax, " << node->number << "\n";
            break;

        case NODE_BINARY_OP:  // unhandled

            generate_code(node->right);
            std::cout << "\tpush rax\n";

            generate_code(node->left);  // saved to rax
            std::cout << "\tpop rbx\n";

            switch (node->op) {
                case '+':
                    std::cout << "\tadd rax, rbx\n";
                    break;
                case '-':
                    std::cout << "\tsub rax ,rbx\n";
                    break;
                case '*':
                    std::cout << "\timul rax, rbx\n";
                    break;
                case '/':
                    std::cout << "\tcqo\n";
                    std::cout << "\tidiv rbx\n";
                    break;
                case '%':
                    std::cout << "\tcqo\n";
                    std::cout << "\tidiv rbx\n";
                    std::cout << "\tmov rax, rdx\n";
                    break;
                default:
                    std::cerr << "error op is not a math symbol\n";
                    exit(1);
            }
            break;
        case NODE_RETURN:  // only supports number, and for now return acts like
                           // exit
            generate_code(node->expression);
            std::cout << "\tpop rbp\n";
            std::cout << "\tret" << '\n';
            break;
        case NODE_FUNCTION:
            std::cout << node->function_name << ":\n";
            std::cout << "\tpush rbp\n";
            std::cout << "\tmov rbp, rsp\n";
            generate_code(node->function_body);
            std::cout << "\tpop rbp\n";
            std::cout << "\tret\n";
            break;
        case NODE_BLOCK:
            for (int i = 0; i < node->statements_count; i++) {
                // std::cerr << "print statement : " << i << '\n';
                generate_code(node->statements[i]);
            }
            break;
        case NODE_NEG:
            generate_code(node->expression);
            std::cout << "\tneg rax\n";
            break;
        case NODE_VARIABLE:
          break;
        }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "please input a file\n";
        return 1;
    }

    FILE* fp = freopen(argv[1], "r", stdin);
    if (fp == NULL) {
        perror("freopen on argv[1] failed");
        return 1;
    }

    FILE* targetfp = freopen("out.asm", "w", stdout);
    if (targetfp == NULL) {
        perror("failed to do freopen on stdout");
        return 1;
    }
    // configure precedence
    precedence['+'] = 1;
    precedence['-'] = 1;
    precedence['*'] = 2;
    precedence['/'] = 2;
    precedence['%'] = 2;

    // --- debugging lexer ---
    // std::cerr << "--- LEXER --- \n";
    // do {
    //     progress_symbol();
    //     print_token(next_symbol.token);
    // } while (next_symbol.token != TOK_EOF);

    // std::cerr << "--- Parser ---\n";
    progress_symbol();
    ASTNode* ast_head = parse_start();
    generate_code(ast_head);
    // print_AST(ast_head, 0);

    fclose(targetfp);
    fclose(fp);
}
