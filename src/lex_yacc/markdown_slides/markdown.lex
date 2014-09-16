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
%x INDENTLIST SHTMLBLOCK

blankline ^[ ]{0,4}\r?\n
quoteblankline ^>[ ]{0,4}\r?\n

%%

{blankline}                             { yylineno++; P("BLANKLINE"); return BLANKLINE; }
    /* !!! note: quoteblankline has no effect */
^>\r?\n                                    { yylineno++; P("QUOTEBLANKLINE"); return QUOTEBLANKLINE; }
^>" "\r?\n                                 { yylineno++; P("QUOTEBLANKLINE"); return QUOTEBLANKLINE; }
^>"  "\r?\n                                { yylineno++; P("QUOTEBLANKLINE"); return QUOTEBLANKLINE; }
^>"   "\r?\n                               { yylineno++; P("QUOTEBLANKLINE"); return QUOTEBLANKLINE; }
^>"    "\r?\n                              { yylineno++; P("QUOTEBLANKLINE"); return QUOTEBLANKLINE; }

^>                                      { P("LARGERTHAN"); return LARGERTHAN; }

^@s.*                                   { P("SECTION"); 
                                            yylval.text = strdup(yytext);
                                            return SECTION; }
^@vs.*                                  { P("VSECTION"); 
                                            yylval.text = strdup(yytext);
                                            return VSECTION; }


\\                                      { BEGIN ESCAPE; }
<ESCAPE>[\\`*_{}()#+\-.!]               { BEGIN INITIAL; yylval.text = strdup(yytext); P("SPECIALCHAR"); return SPECIALCHAR; }
<ESCAPE>.                               { BEGIN INITIAL; yylval.text = strdup(yytext); P("SPECIALCHAR"); return SPECIALCHAR; }

"`"                                     { P("BACKTICK"); BEGIN CODESPAN; return BACKTICK; }
<CODESPAN>\\`                           { P("SPECIALCHAR"); 
                                            yylval.text = strdup("`"); return SPECIALCHAR; }
<CODESPAN>[^`\\\r\n]+                   { P("CODETEXT"); yylval.text = strdup(yytext); return CODETEXT; }
<CODESPAN>\r?\n|\\                      { P("CODETEXT"); yylval.text = strdup(yytext); 
                                            yylineno++; return CODETEXT; }
<CODESPAN>`                             { P("BACKTICK"); BEGIN INITIAL; return BACKTICK; }

"``"                                    { P("DOUBLEBACKTICK"); BEGIN XCODESPAN; return DOUBLEBACKTICK; }
<XCODESPAN>.                            { P("CODETEXT"); yylval.text = strdup(yytext); return CODETEXT; }
<XCODESPAN>\r?\n                        { P("CODETEXT"); yylval.text = strdup(yytext);
                                            yylineno++; return CODETEXT; }
<XCODESPAN>``                           { P("DOUBLEBACKTICK"); BEGIN INITIAL; return DOUBLEBACKTICK; }



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
<CODEBLOCK>\r?\n                        { BEGIN INITIAL; yylineno++; }

<INDENTLIST>[ ]{0,3}[*+][ ]+            { BEGIN INITIAL; P("ULSTART"); return ULSTART; }
<INDENTLIST>[ ]{0,3}[1-9][0-9]*\.[ ]+   { BEGIN INITIAL; P("OLSTART"); return OLSTART; }


^#{1,6}                       { yylval.text = strdup(yytext); P("H"); return H; }
^>" "+#{1,6}                  { yylval.text = strdup(yytext); P("QUOTEH"); return QUOTEH; }

    /* block and functional html tags must be in one line */
^\<\/?(div|table|tr|td|h[1-6]|dl|iframe|section|header|footer|ul|ol|li|script|style|aside)[^>]*\>   { 
                                            yylval.text = strdup(yytext); 
                                            P("HTMLBLOCK"); 
                                            BEGIN SHTMLBLOCK; 
                                            return HTMLBLOCK; 
                                        }
<SHTMLBLOCK>.+          { yylval.text = strdup(yytext); P("TEXT"); return TEXT; }
<SHTMLBLOCK>\r?\n       { yylineno++; P("LINEBREAK"); BEGIN INITIAL;  return LINEBREAK; }


.                             { yylval.text = strdup(yytext); P("TEXT"); return TEXT; }
\r?\n                         { yylineno++; P("LINEBREAK"); return LINEBREAK; }


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



