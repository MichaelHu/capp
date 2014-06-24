%{

#include "test.y.h"

%}

%%

"t"                         { yylval.text = strdup(yytext); return T; }
"*"                         { yylval.text = strdup(yytext); return E; }
"+"                         { yylval.text = strdup(yytext); return P; }
\n                          { yylineno++; }

.                           ; 

%%

int yywrap(){
    return 1;
}

