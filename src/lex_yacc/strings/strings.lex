%{
    /* declare yylval type */
    char *yylval;
    #include <string.h>
    char buf[10000];
    char *s;
%}
%x STRING COMMENT

%%

    /*
    \"[^"\n]*["\n] {
        yylval = strdup(yytext + 1);
        if(yylval[yyleng - 2] != '"'){
            printf("improperly terminated string\n");
            exit(1);
        }
        else{
            yylval[yyleng - 2] = 0;
        }
        printf("found '%s'\n", yylval);
    }

    .|\n ;
    */

'\\?\"'                 ; /* eat '\"' */ 



"/*"                    { BEGIN COMMENT; }
<COMMENT>[^*\n]*        /* eat anything that's not a '*' */
<COMMENT>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<COMMENT>\n             /* eat up '\n' */
<COMMENT>"*"+"/"        { BEGIN INITIAL; }



\"                      { BEGIN STRING; s = buf; } 
<STRING>\\n             { *s++ = '\n'; }
<STRING>\\t             { *s++ = '\t'; }
<STRING>\\\"            { *s++ = '\"'; }
<STRING>\"              {
                            *s = 0;
                            BEGIN INITIAL;
                            printf ("[%s]\n", buf);
                        }
<STRING>\n              { printf("invalid c string\n"); exit(1);}
<STRING>.               { *s++ = *yytext; }



.|\n                    ;


%%

int yywrap(){
    return 1;
}

int main(int argc, char **argv){
    yyin = fopen(argv[1], "r");
    yylex();
    fclose(yyin);
}
