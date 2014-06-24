%{

#include "test.y.h"

%}

%%

"t"                         { yylval.text = strdup(yytext); return T; }
"k"                         { yylval.text = strdup(yytext); return K; }

.|\n                        ; 

%%

int yywrap(){
    return 1;
}

