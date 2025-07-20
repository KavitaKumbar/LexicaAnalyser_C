
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// Keyword List (expand as needed)
const char *keywords[] = {"int","float","return","if","else","while","for","do","break","continue",
    "char","double","void","switch","case","default","const","static","sizeof","struct"
    ,"volatile","long"
};
int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

// Function to check if a string is a keyword
int is_keyword(const char *str)
{
    for (int i = 0; i < num_keywords; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Lexical Analyzer Function
Token *getNextToken(FILE *file)
{
    char token[MAX_TOKEN_SIZE];
    memset(token, 0, MAX_TOKEN_SIZE);
    int tokenLength = 0;
    int string_literal = 0 , string_lite = 0;// Flag for string literals
    char c;
    int singleline = 0, multiline = 0;
    while ((c = fgetc(file)) != EOF)
    {
        if (string_literal)
        {
            if (c == '"')
            {
                string_literal = 0;
                string_lite = 1;
                break; // End of string literal
            }
            else
            {
                token[tokenLength++] = c;
            }
            continue; // Skip to next character in string literal
        }
        if(c == '"')
        {
            string_literal = 1;
            continue;
        }
        if (singleline)
        {
            if (c == '\n')
            {
                singleline = 0;
            }
            continue; // skip single line comment
        }
        if(multiline)
        {
            if(c == '*')
            {
                char next_c = fgetc(file);
                if(next_c == '/')
                {
                    multiline = 0;
                }
            }
            continue;
        }
        if (isspace(c))
        {
            if (tokenLength > 0)
            {
                break; // Token complete
            }
            else
            {
                continue; // Skip leading whitespace
            }
        }

        if (c == '/')
        {
            char next_c = fgetc(file);
            if (next_c == '/')
            {
                singleline = 1;
                continue;
            }
            else if (next_c == '*')
            {
                multiline = 1;
                continue;
            }
        }
        if (strchr("+-*/=<>!&|", c))// Operator
        { 
            if (tokenLength > 0)
            {
                ungetc(c, file);
                break;
            }
            else
            {
                token[tokenLength++] = c;
                char next_ch = fgetc(file);
                if (next_ch == '=')
                {
                    token[tokenLength++] = next_ch;
                }
                else
                {
                    ungetc(next_ch, file); // If not '=', put it back
                }
                break;
            }
        }

        if (strchr("(){}[],:;", c))// Punctuator
        { 
            if (tokenLength > 0)
            {
                ungetc(c, file);
                break;
            }
            else
            {
                token[tokenLength++] = c;
                break;
            }
        }

        if (isalnum(c) || c == '_') // Identifier or keyword
        {
            token[tokenLength++] = c;
        }
        else if (isdigit(c))// Number
        { 
            token[tokenLength++] = c;
        }
        else
        {
            // Error handling 
            fprintf(stderr, "Error: Invalid character '%c'\n", c);
            Token *newToken = (Token *)malloc(sizeof(Token));
            if (newToken == NULL)
            {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            newToken->type = TOKEN_ERROR;
            strcpy(newToken->value, ""); // Clear the token on error.
            return newToken;
        }

        if (tokenLength >= MAX_TOKEN_SIZE - 1)
        {
            fprintf(stderr, "Error: Token too long\n");
            Token *newToken = (Token *)malloc(sizeof(Token));
            if (newToken == NULL)
            {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            newToken->type = TOKEN_ERROR;
            strcpy(newToken->value, ""); // Clear the token on error.
            return newToken;
        }
    }
    if(tokenLength > 0)
    token[tokenLength] = '\0';
    Token *newToken = (Token *)malloc(sizeof(Token));
    if (newToken == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    if (c == EOF)
    {
        newToken->type = TOKEN_EOF;// Should have been
        strcpy(newToken->value,"");
        return newToken;
    }
    strcpy(newToken->value, token);
    if (strlen(token) == 0 && feof(file))
    {
        newToken->type = TOKEN_EOF;
    }
    else if (strlen(token) == 0 && string_literal)// check if string literal is still open
    { 
        newToken->type = TOKEN_ERROR;
        fprintf(stderr, "Error: Unterminated string literal\n");
    }
    else if (strlen(token) == 0)
    {
        newToken->type = TOKEN_ERROR;
    }
    else if (is_keyword(token) && string_lite == 0)
    {
        newToken->type = TOKEN_KEYWORD;
    }
    else if ((isalpha(token[0]) || token[0] == '_') && string_lite == 0)
    {
        newToken->type = TOKEN_IDENTIFIER;
    }
    else if (isdigit(token[0]))
    {
        newToken->type = TOKEN_NUMBER;
    }
    else if (strchr("+-*/=<>!&|", token[0]))
    {
        newToken->type = TOKEN_OPERATOR;
    }
    else if (strchr("(){}[],:;", token[0]))
    {
        newToken->type = TOKEN_PUNCTUATOR;
    }
    else if (string_lite)
    {
        newToken->type = TOKEN_STRING;
    }
    else
    {
        newToken->type = TOKEN_ERROR; // Should have been caught earlier.
    }
    return newToken;
}