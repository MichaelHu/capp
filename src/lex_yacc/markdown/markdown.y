%{
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "htmltags.h" 
#include "blocknode.h" 

/* prototypes */
int yylex(void);
void yyerror(char *s);
FILE *yyin;
int yylineno;
%}


%union{
    char *text;
    t_blocknode *node;
};

    /* bind with terminater */
%token <text> TEXT SPECIALCHAR CODETEXT INDENT H QUOTEH
%token EXCLAMATION MINUS PLUS RIGHTPARENTHESES LEFTPARENTHESES RIGHTSQUARE LEFTSQUARE
%token LEFTCURLY RIGHTCURLY UNDERSCORE STAR BACKTICK BLANKLINE LINEBREAK LARGERTHAN
%token DOUBLESTAR DOUBLEUNDERSCORE OLSTART ULSTART DOUBLEBACKTICK QUOTEBLANKLINE QUOTEOLSTART QUOTEULSTART

%type <text> inlineelements inlineelement plaintext text_list
%type <text> codespan code_list error lines 
%type <node> line

%nonassoc TEXT SPECIALCHAR EXCLAMATION LEFTSQUARE STAR DOUBLESTAR UNDERSCORE DOUBLEUNDERSCORE BACKTICK DOUBLEBACKTICK LEFTPARENTHESES RIGHTSQUARE RIGHTPARENTHESES error
%nonassoc STARX

%%

markdownfile: 
    lines { /* tag_show_stack(); */ blocklist_parse(); }
    ;

lines:
    lines line  { 
            /* $$ = str_concat($1, blocknode_parse($2)); */ 
        }

    | /* NULL */{ $$ = ""; }
    ;

line:
    BLANKLINE { 
            tag_check_stack(TAG_BLANK, 0); 
            $$ = blocknode_create(TAG_BLANK, 0, 1, "");
        }

    | QUOTEBLANKLINE { 
            tag_check_stack(TAG_QUOTE_BLANK, 0); 
            $$ = blocknode_create(TAG_QUOTE_BLANK, 0, 1, "");
        }

    | H plaintext LINEBREAK {              
            tag_check_stack(TAG_H, 0); 
            $$ = blocknode_create(TAG_H, 0, 2, $1, $2);
        }   
    | QUOTEH plaintext LINEBREAK { 
            tag_check_stack(TAG_QUOTE_H, 0); 
            $$ = blocknode_create(TAG_QUOTE_H, 0, 2, $1, $2);
        }   


    | inlineelements LINEBREAK { 
            tag_check_stack(TAG_P, 0); 
            $$ = blocknode_create(TAG_P, 0, 1, $1);
        } 

    | LARGERTHAN inlineelements LINEBREAK { 
            tag_check_stack(TAG_QUOTE_P, 0); 
            $$ = blocknode_create(TAG_QUOTE_P, 0, 1, $2);
        } 

    | OLSTART inlineelements LINEBREAK { 
            tag_check_stack(TAG_OL, 0); 
            $$ = blocknode_create(TAG_OL, 0, 1, $2);
        } 

    | QUOTEOLSTART inlineelements LINEBREAK { 
            tag_check_stack(TAG_QUOTE_OL, 0); 
            $$ = blocknode_create(TAG_QUOTE_OL, 0, 1, $2);
        } 

    | INDENT OLSTART inlineelements LINEBREAK { 
            tag_check_stack(TAG_INDENT_OL, indent_level($1)); 
            $$ = blocknode_create(TAG_INDENT_OL, indent_level($1), 2, $1, $3);
        } 

    | ULSTART inlineelements LINEBREAK { 
            tag_check_stack(TAG_UL, 0); 
            $$ = blocknode_create(TAG_UL, 0, 1, $2);
        } 

    | INDENT ULSTART inlineelements LINEBREAK { 
            tag_check_stack(TAG_INDENT_UL, indent_level($1)); 
            $$ = blocknode_create(TAG_INDENT_UL, indent_level($1), 2, $1, $3);
        } 

    | QUOTEULSTART inlineelements LINEBREAK { 
            tag_check_stack(TAG_QUOTE_UL, 0); 
            $$ = blocknode_create(TAG_QUOTE_UL, 0, 1, $2);
        } 

    | INDENT inlineelements LINEBREAK { 
            tag_check_stack(TAG_INDENT_P, indent_level($1)); 
            $$ = blocknode_create(TAG_INDENT_P, indent_level($1), 2, $1, $2);
        } 
    | INDENT CODETEXT {
            if(is_inner_pre(indent_level($1))){
                /* PRE indent level is 1 less than the literal indent */
                tag_check_stack(TAG_INDENT_PRE, indent_level($1) - 1); 
                $$ = blocknode_create(TAG_INDENT_PRE, indent_level($1) - 1, 2, $1, $2);
            }
            else{
                tag_check_stack(TAG_PRE, indent_level($1) - 1); 
                $$ = blocknode_create(TAG_PRE, indent_level($1) - 1, 2, $1, $2);
            }
        }

    | error LINEBREAK { 
            $$ = blocknode_create(TAG_NULL, 0, 1, str_format("%s", "@error@")); 
            yyerrok; 
            yyclearin; 
        }
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




