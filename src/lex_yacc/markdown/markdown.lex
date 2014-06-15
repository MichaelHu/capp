%{

#include <string.h>
#include "markdown.y.h"

/* prototypes */
void yyerror(char *s);

%}

%x ESCAPE

blankline ^[ \t]*\n

%%

{blankline}                             { return BLANKLINE; }

^>                                      { return LARGERTHAN; }

\\                                      { BEGIN ESCAPE; }
<ESCAPE>[\\`*_{}()#+\-.!]               { BEGIN INITIAL; yylval.text = strdup(yytext); return TEXT; }
<ESCAPE>.                               { BEGIN INITIAL; yylval.text = strdup(yytext); return TEXT; }

"`"                                     { return BACKTICK; }
"*"                                     { return STAR; }
"_"                                     { return UNDERSCORE; }
"{"                                     { return LEFTCURLY; }
"}"                                     { return RIGHTCURLY; }
"["                                     { return LEFTSQUARE; }
"]"                                     { return RIGHTSQUARE; }
"("                                     { return LEFTPARENTHESES; }
")"                                     { return RIGHTPARENTHESES; }
"+"                                     { return PLUS; }
"-"                                     { return MINUS; }
    /*
    "."                                     { return DOT; }
    */
"!"                                     { return EXCLAMATION; }


^#                       { return H1; }
^##                      { return H2; }
^###                     { return H3; }
^####                    { return H4; }
^#####                   { return H5; }
^######                  { return H6; }

[^#!\-+()\[\]{}_*`\\>\n]+  { yylval.text = strdup(yytext); return TEXT; }
\n                       { return LINEBREAK; }


%%

int yywrap(){
    return 1;
}

    /*
    int main(int argc, char **argv){
        yyin = fopen(argv[1], "r");
        yylex();
        fclose(yyin);
    }
    */



