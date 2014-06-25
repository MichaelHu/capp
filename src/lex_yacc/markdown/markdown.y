%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htmltags.h" 

/* prototypes */
int yylex(void);
void yyerror(char *s);
FILE *yyin;
int yylineno;
%}


%union{
    char *text;        /* symbol table index */
};

    /* bind with terminater */
%token <text> TEXT SPECIALCHAR CODETEXT
%token H1 H2 H3 H4 H5 H6 
%token EXCLAMATION MINUS PLUS RIGHTPARENTHESES LEFTPARENTHESES RIGHTSQUARE LEFTSQUARE
%token LEFTCURLY RIGHTCURLY UNDERSCORE STAR BACKTICK BLANKLINE LINEBREAK LARGERTHAN
%token DOUBLESTAR DOUBLEUNDERSCORE OLSTART ULSTART DOUBLEBACKTICK

%type <text> lines line inlineelements inlineelement plaintext text_list
%type <text> codespan code_list 

%nonassoc TEXT SPECIALCHAR EXCLAMATION LEFTSQUARE STAR DOUBLESTAR UNDERSCORE DOUBLEUNDERSCORE BACKTICK DOUBLEBACKTICK
%nonassoc STARX

%%

markdownfile: 
    lines                       { printf("%s\n", $1); }
    ;

lines:
    lines line                  { $$ = str_concat($1, $2); }
    | /* NULL */                { $$ = ""; }
    ;

line:
      BLANKLINE                { $$ = ""; }
    | H1 plaintext LINEBREAK                  { $$ = create_hn($2, 1); }  
    | H2 plaintext LINEBREAK                  { $$ = create_hn($2, 2); }   
    | H3 plaintext LINEBREAK                  { $$ = create_hn($2, 3); }   
    | H4 plaintext LINEBREAK                  { $$ = create_hn($2, 4); }  
    | H5 plaintext LINEBREAK                  { $$ = create_hn($2, 5); }  
    | H6 plaintext LINEBREAK                  { $$ = create_hn($2, 6); }   
    | inlineelements LINEBREAK            { $$ = str_format("<p>%s</p>\n", $1); } 
    | OLSTART inlineelements LINEBREAK            { $$ = str_format("<li>%s</li>\n", $2); } 
    | ULSTART inlineelements LINEBREAK            { $$ = str_format("<li>%s</li>\n", $2); } 
    | error LINEBREAK                     { $$ = ""; }
    ;

inlineelements:  
    inlineelements inlineelement                        { $$ = str_concat($1, $2); }
    | inlineelement                     { $$ = $1; }
    ;

inlineelement:
    TEXT                                { $$ = $1; }
    | SPECIALCHAR                       { $$ = $1; }

    | STAR inlineelements STAR %prec STARX              { $$ = create_emphasis($2); } 
    | UNDERSCORE inlineelements UNDERSCORE %prec STARX             { $$ = create_emphasis($2); } 
    | DOUBLESTAR inlineelements DOUBLESTAR %prec STARX              { $$ = create_strong($2); }
    | DOUBLEUNDERSCORE inlineelements DOUBLEUNDERSCORE %prec STARX  { $$ = create_strong($2); }

    | BACKTICK codespan BACKTICK        { $$ = create_codespan($2); }
    | DOUBLEBACKTICK codespan DOUBLEBACKTICK        { $$ = create_codespan($2); }

    | LEFTSQUARE plaintext RIGHTSQUARE LEFTPARENTHESES plaintext RIGHTPARENTHESES {
                                 $$ = create_link($2, $5);
                                } 
    | EXCLAMATION LEFTSQUARE plaintext RIGHTSQUARE LEFTPARENTHESES plaintext RIGHTPARENTHESES {
                                 $$ = create_image($3, $6);
                                } 
    ;

plaintext:
    plaintext text_list             { $$ = str_concat($1, $2); }
    | text_list                     { $$ = $1; }
    ;

text_list:
    TEXT                        { $$ = str_format("%s", $1); }
    | SPECIALCHAR                   { $$ = str_format("%s", $1); }
    ;

codespan:
    codespan code_list             { $$ = str_concat($1, $2); }
    | code_list                     { $$ = $1; }
    ;

code_list:
    CODETEXT                        { $$ = str_format("%s", $1); }
    | SPECIALCHAR                   { $$ = str_format("%s", $1); }
    ;

%%

void yyerror(char *s) {
    fprintf(stdout, "line %d: %s\n", yylineno, s);
}

int main(int argc, char **argv){
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
}




