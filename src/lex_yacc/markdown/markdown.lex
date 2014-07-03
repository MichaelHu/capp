%{

#include <string.h>
#include "blocknode.h"
#include "markdown.y.h"

/* prototypes */
void yyerror(char *s);
int yylineno;

%}

%x ESCAPE CODESPAN XCODESPAN CODEBLOCK
%x INDENTLIST 

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


^" "{0,3}[*+-][ ]+                      { return ULSTART; }
^>" "+[*+-][ ]+                         { return QUOTEULSTART; }
^" "{0,3}[1-9][0-9]*\.[ ]+              { return OLSTART; }
^>" "+[1-9][0-9]*\.[ ]+                 { return QUOTEOLSTART; }



^(\t|[ ]{4})+/[ ]{0,3}\*[ ]+            { 
                                            /* indent list */
                                            if(latest_list_level() + 1 == indent_level(yytext)){
                                                BEGIN INDENTLIST;
                                                yylval.text = strdup(yytext);
                                                return INDENT; 
                                            }
                                            else{
                                                BEGIN CODEBLOCK; 
                                                yylval.text = strdup(yytext);
                                                return INDENT;
                                            }
                                        }   
^(\t|[ ]{4})+                           { 
                                            /* indent p */
                                            if(latest_list_level() + 1 == indent_level(yytext)){
                                                yylval.text = strdup(yytext);
                                                return INDENT; 
                                            }
                                            else{
                                                BEGIN CODEBLOCK; 
                                                yylval.text = strdup(yytext);
                                                return INDENT;
                                            }
                                        }
<CODEBLOCK>.+                           { yylval.text = strdup(yytext); return CODETEXT; }
<CODEBLOCK>\n                           { BEGIN INITIAL; yylineno++; }

<INDENTLIST>[ ]{0,3}\*[ ]+              { BEGIN INITIAL; return ULSTART; }

    /*
    ^(\t|[ ]{4}){2}                         { yylval.text = strdup(yytext); return INDENT2; }
    ^(\t|[ ]{4}){3}                         { yylval.text = strdup(yytext); return INDENT3; }
    */

    /*
    ^(\t|[ ]{4})+                           { BEGIN CODEBLOCK; yylval.text = strdup(yytext); return INDENT; }
    <CODEBLOCK>.+                           { yylval.text = strdup(yytext); return CODETEXT; }
    <CODEBLOCK>\n                           { BEGIN INITIAL; yylineno++; }
    */


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


^#{1,6}                       { yylval.text = strdup(yytext); return H; }
^>" "+#{1,6}                  { yylval.text = strdup(yytext); return QUOTEH; }



[^#!+()\[\]{}_*`\\\n\t" ".]+  { yylval.text = strdup(yytext); return TEXT; }
[.#!+()\[\]{}_*`\\\t" "]      { yylval.text = strdup(yytext); return TEXT; }
\n                            { yylineno++; return LINEBREAK; }


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



