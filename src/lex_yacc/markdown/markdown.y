%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htmltags.h" 

/* prototypes */
int yylex(void);
void yyerror(char *s);
FILE *yyin;
%}


%union{
    char *text;        /* symbol table index */
};

    /* bind with terminater */
%token <text> TEXT
%token H1 H2 H3 H4 H5 H6 
%token EXCLAMATION DOT MINUS PLUS RIGHTPARENTHESES LEFTPARENTHESES RIGHTSQUARE LEFTSQUARE
%token LEFTCURLY RIGHTCURLY UNDERSCORE STAR BACKTICK BLANKLINE LINEBREAK LARGERTHAN 

%type <text> lines line inlineelements inlineelement

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
    | H1 TEXT LINEBREAK                  { $$ = create_hn($2, 1); }  
    | H2 TEXT LINEBREAK                  { $$ = create_hn($2, 2); }   
    | H3 TEXT LINEBREAK                  { $$ = create_hn($2, 3); }   
    | H4 TEXT LINEBREAK                  { $$ = create_hn($2, 4); }  
    | H5 TEXT LINEBREAK                  { $$ = create_hn($2, 5); }  
    | H6 TEXT LINEBREAK                  { $$ = create_hn($2, 6); }   
    | inlineelements LINEBREAK            { $$ = str_format("<p>%s</p>", $1); } 
    ;

inlineelements:  
    inlineelements inlineelement        { $$ = str_concat($1, $2); }
    | /* NULL */                { $$ = ""; }
    ;

inlineelement:
    TEXT                                 { $$ = $1; }
    | LEFTSQUARE TEXT RIGHTSQUARE LEFTPARENTHESES TEXT RIGHTPARENTHESES {
                                 $$ = create_link($2, $5);
                                } 
    | EXCLAMATION LEFTSQUARE TEXT RIGHTSQUARE LEFTPARENTHESES TEXT RIGHTPARENTHESES {
                                 $$ = create_image($3, $6);
                                } 

%%

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(int argc, char **argv){
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
}




