#include "tok.h"
//#include "exp.h"
//#include "stm.h"
#include "parse.h"

#include <stdio.h>
#include <string.h>



int main()
{
    char exp[1024];
    int exp_len;
    struct token_t *tokens;

    int short a;

    while(1)
    {
        memset(exp, 0, 1024);
        fgets(exp, 1024, stdin);

        exp_len = strlen(exp);

        if(exp_len < 1)
        {
            break;
        }

        exp[exp_len - 1] = '\0';

        parse(exp);

//        tokens = tokenize(exp);


//        parse_tokens(tokens);
        //token = tokens;

        //ast = exp_16(&token);
        //traverse_ast(ast);

        //parse_statements(exp);
    }
}
