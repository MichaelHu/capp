%{
    #include <stdlib.h>
    void lexerror(char *);

    /* header file which includes token #defines is produced by yacc */
    #include "calc.y.h"
%} 




%%

    /* variables */
[a-z] {
    yylval = *yytext - 'a';
    return VARIABLE;
}

    /* integers */
[0-9]+ {
    yylval = atoi(yytext);
    return INTEGER;
}

    /* operators */
[-+()=/*\n] { return *yytext; }

    /* skip whitespace */
[ \t] ;

    /* anything else is an error */
. lexerror("invalid character");



%%

void lexerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int yywrap(void) {
    return 1;
}
