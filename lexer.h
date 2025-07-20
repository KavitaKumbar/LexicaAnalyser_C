#ifndef LEXER_H
#define LEXER_H

#define MAX_TOKEN_SIZE 100

// Token Types (enum)
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATOR,
    TOKEN_STRING, // For string literals
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

// Token Structure
typedef struct {
    TokenType type;
    char value[MAX_TOKEN_SIZE];
} Token;
// Function Prototypes
Token* getNextToken(FILE *file);
int is_keyword(const char *str);

#endif // LEXER_H