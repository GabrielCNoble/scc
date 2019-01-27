#include "token.h"
#include "exp.h"

#include <stdio.h>
#include <string.h>


int main()
{
//    struct token_t *tokens;
    char exp[1024];
    int exp_len;

    while(1)
    {
        //printf("Type some bullshit...\n");
        memset(exp, 0, 1024);
        fgets(exp, 1024, stdin);

        exp_len = strlen(exp);

        if(exp_len == 1)
        {
            break;
        }

        exp[exp_len - 1] = '\0';

        parse_statement(exp);

//        tokens = tokenize(exp);
//        exp_start(tokens);
//        free_tokens(tokens);
    }
}
