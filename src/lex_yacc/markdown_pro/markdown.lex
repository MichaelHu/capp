%{

#include <string.h>
#include "blocknode.h"
#include "markdown.y.h"

#undef _ISDEBUGLEX

#ifdef _ISDEBUGLEX
#define P(token) printf("token: %s\n", token);
#else
#define P(token)
#endif

/* prototypes */
void yyerror(char *s);
int yylineno;

%}

%x ESCAPE CODEBLOCK CODESPAN XCODESPAN 
%x INDENTLIST

blankline ^[ ]{0,3}\n
quoteblankline ^>[ ]{0,3}\n

%%

{blankline}                             { yylineno++; P("BLANKLINE"); return BLANKLINE; }
    /* !!! note: quoteblankline has no effect */
^>\n                                    { yylineno++; P("QUOTEBLANKLINE"); return QUOTEBLANKLINE; }
^>" "\n                                 { yylineno++; P("QUOTEBLANKLINE"); return QUOTEBLANKLINE; }
^>"  "\n                                { yylineno++; P("QUOTEBLANKLINE"); return QUOTEBLANKLINE; }
^>"   "\n                               { yylineno++; P("QUOTEBLANKLINE"); return QUOTEBLANKLINE; }

^>                                      { P("LARGERTHAN"); return LARGERTHAN; }


\\                                      { BEGIN ESCAPE; }
<ESCAPE>[\\`*_{}()#+\-.!]               { BEGIN INITIAL; yylval.text = strdup(yytext); P("SPECIALCHAR"); return SPECIALCHAR; }
<ESCAPE>.                               { BEGIN INITIAL; yylval.text = strdup(yytext); P("SPECIALCHAR"); return SPECIALCHAR; }

"`"                                     { BEGIN CODESPAN; return BACKTICK; }
<CODESPAN>\\`                           { yylval.text = strdup(yytext); return SPECIALCHAR; }
<CODESPAN>`                             { BEGIN INITIAL; return BACKTICK; }
<CODESPAN>[^`\n]+                       { yylval.text = strdup(yytext); return CODETEXT; }

"``"                                    { BEGIN XCODESPAN; return DOUBLEBACKTICK; }
<XCODESPAN>``                           { BEGIN INITIAL; return DOUBLEBACKTICK; }
<XCODESPAN>.                            { yylval.text = strdup(yytext); return CODETEXT; }



^" "{0,3}[*+-][ ]+                      { P("ULSTART"); return ULSTART; }
^>" "+[*+-][ ]+                         { P("QUOTEULSTART"); return QUOTEULSTART; }
^" "{0,3}[1-9][0-9]*\.[ ]+              { P("OLSTART"); return OLSTART; }
^>" "+[1-9][0-9]*\.[ ]+                 { P("QUOTEOLSTART"); return QUOTEOLSTART; }



^(\t|[ ]{4})+/[ ]{0,3}[*+][ ]+            { 
                                            /* indent ul list */
                                            if(is_in_list(indent_level(yytext))){
                                                BEGIN INDENTLIST;
                                                yylval.text = strdup(yytext);
                                                P("INDENT"); return INDENT; 
                                            }
                                            else{
                                                BEGIN CODEBLOCK; 
                                                yylval.text = strdup(yytext);
                                                P("INDENT"); return INDENT;
                                            }
                                        }   
^(\t|[ ]{4})+/[ ]{0,3}[1-9][0-9]*\.[ ]+ { 
                                            /* indent ol list */
                                            if(is_in_list(indent_level(yytext))){
                                                BEGIN INDENTLIST;
                                                yylval.text = strdup(yytext);
                                                P("INDENT"); return INDENT; 
                                            }
                                            else{
                                                BEGIN CODEBLOCK; 
                                                yylval.text = strdup(yytext);
                                                P("INDENT"); return INDENT;
                                            }
                                        }   
^(\t|[ ]{4})+/.                         { 
                                            /* indent p */
                                            if(is_in_list(indent_level(yytext))){
                                                yylval.text = strdup(yytext);
                                                P("INDENT"); return INDENT; 
                                            }
                                            else{
                                                BEGIN CODEBLOCK; 
                                                yylval.text = strdup(yytext);
                                                P("INDENT"); return INDENT;
                                            }
                                        }
<CODEBLOCK>.+                           { yylval.text = strdup(yytext); P("CODETEXT"); return CODETEXT; }
<CODEBLOCK>\n                           { BEGIN INITIAL; yylineno++; }

<INDENTLIST>[ ]{0,3}[*+][ ]+            { BEGIN INITIAL; P("ULSTART"); return ULSTART; }
<INDENTLIST>[ ]{0,3}[1-9][0-9]*\.[ ]+   { BEGIN INITIAL; P("OLSTART"); return OLSTART; }


^#{1,6}                       { yylval.text = strdup(yytext); P("H"); return H; }
^>" "+#{1,6}                  { yylval.text = strdup(yytext); P("QUOTEH"); return QUOTEH; }


.                             { yylval.text = strdup(yytext); P("TEXT"); return TEXT; }
\n                            { yylineno++; P("LINEBREAK"); return LINEBREAK; }


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



