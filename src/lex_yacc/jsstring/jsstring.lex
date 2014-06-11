%{
    /* declare yylval type */
    char *yylval;
    #include <string.h>
    char buf[10000];
    char *s;
%}
%x STRING COMMENT SQSTRING SLCOMMENT

%%

"//"                    { BEGIN SLCOMMENT; }
<SLCOMMENT>.            ;
<SLCOMMENT>\n           { BEGIN INITIAL; }


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
<STRING>\n              { printf("invalid js string\n"); exit(1);}
<STRING>.               { *s++ = *yytext; }



\'                      { BEGIN SQSTRING; s = buf; } 
<SQSTRING>\\n           { *s++ = '\n'; }
<SQSTRING>\\t           { *s++ = '\t'; }
<SQSTRING>\\\'          { *s++ = '\''; }
<SQSTRING>\'            {
                            *s = 0;
                            BEGIN INITIAL;
                            printf ("[%s]\n", buf);
                        }
<SQSTRING>\n            { printf("invalid js string\n"); exit(1);}
<SQSTRING>.             { *s++ = *yytext; }



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

