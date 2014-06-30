%{

#include <string.h>
#include "markdown.y.h"

/* prototypes */
void yyerror(char *s);
int yylineno;

%}

%x ESCAPE CODESPAN XCODESPAN CODEBLOCK

blankline ^[ \t]*\n
quoteblankline ^>[ \t]*\n

%%

{blankline}                             { yylineno++; return BLANKLINE; }
{quoteblankline}                        { yylineno++; return QUOTEBLANKLINE; }

^>                                      { return LARGERTHAN; }


\\                                      { BEGIN ESCAPE; }
<ESCAPE>[\\`*_{}()#+\-.!]               { BEGIN INITIAL; yylval.text = strdup(yytext); return SPECIALCHAR; }
<ESCAPE>.                               { BEGIN INITIAL; yylval.text = strdup(yytext); return SPECIALCHAR; }


"`"                                     { BEGIN CODESPAN; return BACKTICK; }
<CODESPAN>\\`                           { yylval.text = strdup(yytext); return SPECIALCHAR; }
<CODESPAN>`                             { BEGIN INITIAL; return BACKTICK; }
<CODESPAN>[^`\n]+                       { yylval.text = strdup(yytext); return CODETEXT; }


"``"                                    { BEGIN XCODESPAN; return DOUBLEBACKTICK; }
<XCODESPAN>``                           { BEGIN INITIAL; return DOUBLEBACKTICK; }
<XCODESPAN>.                            { yylval.text = strdup(yytext); return CODETEXT; }


^[*+-][ ]+                              { return ULSTART; }
^>" "+[*+-][ ]+                         { return QUOTEULSTART; }
^[1-9][0-9]*\.[ ]+                      { return OLSTART; }
^>" "+[1-9][0-9]*\.[ ]+                 { return QUOTEOLSTART; }


^(\t|[ ]{4})+                           { BEGIN CODEBLOCK; yylval.text = strdup(yytext); return INDENT; }
<CODEBLOCK>.+                           { yylval.text = strdup(yytext); return CODETEXT; }
<CODEBLOCK>\n                           { BEGIN INITIAL; yylineno++; }


"*"                                     { return STAR; }
"_"                                     { return UNDERSCORE; }
"{"                                     { return LEFTCURLY; }
"}"                                     { return RIGHTCURLY; }
"["                                     { return LEFTSQUARE; }
"]"                                     { return RIGHTSQUARE; }
"("                                     { return LEFTPARENTHESES; }
")"                                     { return RIGHTPARENTHESES; }
"+"                                     { return PLUS; }
    /*
    "-"                                     { return MINUS; }
    */

__                                      { return DOUBLEUNDERSCORE; }
\*\*                                    { return DOUBLESTAR; }

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

^>" "+#                       { return QUOTEH1; }
^>" "+##                      { return QUOTEH2; }
^>" "+###                     { return QUOTEH3; }
^>" "+####                    { return QUOTEH4; }
^>" "+#####                   { return QUOTEH5; }
^>" "+######                  { return QUOTEH6; }



[^#!+()\[\]{}_*`\\\n\t" ".]+  { yylval.text = strdup(yytext); return TEXT; }
[.\t" "]                       { yylval.text = strdup(yytext); return TEXT; }
\n                              { yylineno++; return LINEBREAK; }


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



