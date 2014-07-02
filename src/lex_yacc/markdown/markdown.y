%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htmltags.h" 
#include "tagstack.h" 

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
%token <text> TEXT SPECIALCHAR CODETEXT INDENT
%token H1 H2 H3 H4 H5 H6
%token QUOTEH1 QUOTEH2 QUOTEH3 QUOTEH4 QUOTEH5 QUOTEH6 
%token EXCLAMATION MINUS PLUS RIGHTPARENTHESES LEFTPARENTHESES RIGHTSQUARE LEFTSQUARE
%token LEFTCURLY RIGHTCURLY UNDERSCORE STAR BACKTICK BLANKLINE LINEBREAK LARGERTHAN
%token DOUBLESTAR DOUBLEUNDERSCORE OLSTART ULSTART DOUBLEBACKTICK QUOTEBLANKLINE QUOTEOLSTART QUOTEULSTART

%type <text> lines line inlineelements inlineelement plaintext text_list
%type <text> codespan code_list error 

%nonassoc TEXT SPECIALCHAR EXCLAMATION LEFTSQUARE STAR DOUBLESTAR UNDERSCORE DOUBLEUNDERSCORE BACKTICK DOUBLEBACKTICK LEFTPARENTHESES RIGHTSQUARE RIGHTPARENTHESES error
%nonassoc STARX

%%

markdownfile: 
    lines                       { tag_show_stack(); }
    ;

lines:
    lines line                  { $$ = str_concat($1, $2); }
    | /* NULL */                { $$ = ""; }
    ;

line:
      BLANKLINE                { $$ = str_format("%s", tag_check_stack(TAG_BLANK, 0)); }
    | QUOTEBLANKLINE           { $$ = str_format("%s", tag_check_stack(TAG_QUOTE_BLANK, 0)); }

    | H1 plaintext LINEBREAK                  { $$ = create_hn($2, 1); tag_check_stack(TAG_H, 0); }  
    | QUOTEH1 plaintext LINEBREAK             { 
            $$ = str_format("%s%s", tag_check_stack(TAG_QUOTE_H, 0), create_hn($2, 1)); 
        }   

    | H2 plaintext LINEBREAK                  { $$ = create_hn($2, 2); tag_check_stack(TAG_H, 0); }   
    | QUOTEH2 plaintext LINEBREAK             { 
            $$ = str_format("%s%s", tag_check_stack(TAG_QUOTE_H, 0), create_hn($2, 2)); 
        }   

    | H3 plaintext LINEBREAK                  { $$ = create_hn($2, 3); tag_check_stack(TAG_H, 0); }   
    | QUOTEH3 plaintext LINEBREAK             { 
            $$ = str_format("%s%s", tag_check_stack(TAG_QUOTE_H, 0), create_hn($2, 3)); 
        }   

    | H4 plaintext LINEBREAK                  { $$ = create_hn($2, 4); tag_check_stack(TAG_H, 0); }  
    | QUOTEH4 plaintext LINEBREAK             { 
            $$ = str_format("%s%s", tag_check_stack(TAG_QUOTE_H, 0), create_hn($2, 4)); 
        }   

    | H5 plaintext LINEBREAK                  { $$ = create_hn($2, 5); tag_check_stack(TAG_H, 0);  }  
    | QUOTEH5 plaintext LINEBREAK             { 
            $$ = str_format("%s%s", tag_check_stack(TAG_QUOTE_H, 0), create_hn($2, 5)); 
        }   

    | H6 plaintext LINEBREAK                  { $$ = create_hn($2, 6); tag_check_stack(TAG_H, 0); }   
    | QUOTEH6 plaintext LINEBREAK             { 
            $$ = str_format("%s%s", tag_check_stack(TAG_QUOTE_H, 0), create_hn($2, 6)); 
        }   


    | inlineelements LINEBREAK            { 
            $$ = str_format("%s%s\n", tag_check_stack(TAG_P, 0), $1); 
        } 

    | LARGERTHAN inlineelements LINEBREAK            { 
            $$ = str_format("%s%s\n", tag_check_stack(TAG_QUOTE_P, 0), $2); 
        } 

    | OLSTART inlineelements LINEBREAK            { 
            $$ = str_format("%s<li>%s</li>\n", tag_check_stack(TAG_OL, 0), $2); 
        } 

    | QUOTEOLSTART inlineelements LINEBREAK            { 
            $$ = str_format("%s<li>%s</li>\n", tag_check_stack(TAG_QUOTE_OL, 0), $2); 
        } 

    | ULSTART inlineelements LINEBREAK            { 
            $$ = str_format("%s<li>%s\n", tag_check_stack(TAG_UL, 0), $2); 
        } 

    | INDENT ULSTART inlineelements LINEBREAK            { 
            $$ = str_format("%s<li>%s\n", tag_check_stack(TAG_INDENT_UL, indent_level($1)), $3); 
        } 

    | QUOTEULSTART inlineelements LINEBREAK            { 
            $$ = str_format("%s<li>%s\n", tag_check_stack(TAG_QUOTE_UL, 0), $2); 
        } 

    | INDENT inlineelements LINEBREAK            { 
            $$ = str_format("%s%s\n", tag_check_stack(TAG_INDENT_P, indent_level($1)), $2); 
        } 
    | INDENT CODETEXT                           {
            $$ = str_format("%s%s", tag_check_stack(TAG_PRE, 0), html_escape($2) ); 
        }

    | error LINEBREAK                           { $$ = str_format("%s", $1); yyerrok; yyclearin; }
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

    | BACKTICK codespan BACKTICK        { $$ = create_codespan( html_escape($2) ); }
    | DOUBLEBACKTICK codespan DOUBLEBACKTICK        { $$ = create_codespan($2); }

    | LEFTSQUARE plaintext RIGHTSQUARE LEFTPARENTHESES plaintext RIGHTPARENTHESES {
                                 $$ = create_link($2, $5);
                                } 
    | EXCLAMATION LEFTSQUARE plaintext RIGHTSQUARE LEFTPARENTHESES plaintext RIGHTPARENTHESES {
                                 $$ = create_image($3, $6);
                                } 
    | BACKTICK error LINEBREAK          { $$ = str_format("*%s*", $2); yyerrok; yyclearin; }
    | DOUBLEBACKTICK error LINEBREAK          { $$ = str_format("*%s*", $2); yyerrok; yyclearin; }
    | LEFTPARENTHESES error LINEBREAK         { $$ = str_format("*%s*", $2); yyerrok; yyclearin; }
    | RIGHTPARENTHESES error LINEBREAK         { $$ = str_format("*%s*", $2); yyerrok; yyclearin; }
    | LEFTSQUARE error LINEBREAK         { $$ = str_format("*%s*", $2); yyerrok; yyclearin; }
    | RIGHTSQUARE error LINEBREAK         { $$ = str_format("*%s*", $2); yyerrok; yyclearin; }
    | EXCLAMATION error LINEBREAK         { $$ = str_format("*%s*", $2); yyerrok; yyclearin; }
    | STAR error LINEBREAK                              { $$ = str_format("*%s*", $2); yyerrok; yyclearin; }
    | DOUBLESTAR error LINEBREAK                  { $$ = str_format("**%s**", $2); yyerrok; yyclearin; }
    | UNDERSCORE error LINEBREAK                  { $$ = str_format("_%s_", $2); yyerrok; yyclearin; }
    | DOUBLEUNDERSCORE error LINEBREAK      { $$ = str_format("__%s__", $2); yyerrok; yyclearin; }
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
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}

int main(int argc, char **argv){
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
}




