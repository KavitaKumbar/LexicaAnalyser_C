#include <stdio.h>
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
FILE *file;int line = 1,col = 1 ;
int main(int argc,char*argv[]) {
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    Token *token;
    int i = 0;
    char type[50];
    while ((token = getNextToken(fp)) ->type != TOKEN_EOF) 
    {
        if (token->type != TOKEN_EOF && token->type != TOKEN_ERROR) {
            // printf("Type: %d, Value: %s\n", token->type, token->value);
            memset(type, 0,50);
            switch(token -> type)
            {
                case TOKEN_KEYWORD:
                strcpy(type,"Keyword");
                break;
                case TOKEN_IDENTIFIER:
                strcpy(type,"Identifier");
                break;
                case TOKEN_NUMBER:
                strcpy(type,"Number");
                break;
                case TOKEN_OPERATOR:
                strcpy(type,"Operator");
                break;
                case TOKEN_PUNCTUATOR:
                strcpy(type,"Punctuator");
                break;
                case TOKEN_STRING:
                strcpy(type,"String literal");
            }
            printf("Type: %s:      %s\n",type, token->value);
        }
        free(token); 
    }
    fclose(fp);
    return 0;
}