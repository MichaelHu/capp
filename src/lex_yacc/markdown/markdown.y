%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* prototypes */
int yylex(void);
void yyerror(char *s);
FILE *yyin;
char *str_concat(char *s1, char *s2);
char *create_str(char *s, char *format);

%}


%union{
    char *text;        /* symbol table index */
};

    /* bind with terminater */
%token <text> TEXT
%token H1 H2 H3 H4 H5 H6 
%token EXCLAMATION DOT MINUS PLUS RIGHTPARENTHESES LEFTPARENTHESES RIGHTSQUARE LEFTSQUARE
%token LEFTCURLY RIGHTCURLY UNDERSCORE STAR BACKTICK BLANKLINE LINEBREAK LARGERTHAN 

%type <text> lines line

%%

markdownfile: 
    lines                       { printf("output: %s\n", $1); }
    ;

lines:
    lines line                  { $$ = str_concat($1, $2); }
    | /* NULL */                { $$ = ""; }
    ;

line:
      BLANKLINE                { $$ = ""; }
    | H1 TEXT                  { $$ = create_str($2, "<h1>%s</h1>"); }  
    | H2 TEXT                  { $$ = create_str($2, "<h2>%s</h2>"); }   
    | H3 TEXT                  { $$ = create_str($2, "<h3>%s</h3>"); }   
    | H4 TEXT                  { $$ = create_str($2, "<h4>%s</h4>"); }  
    | H5 TEXT                  { $$ = create_str($2, "<h5>%s</h5>"); }  
    | H6 TEXT                  { $$ = create_str($2, "<h6>%s</h6>"); }   
    ;


%%

char *str_concat(char *s1, char *s2){
    char *_str;

    _str = (char *)malloc(strlen(s1) + strlen(s2) + 1);     
    sprintf(_str, "%s%s", s1, s2); 
    return _str;
}

char *create_str(char *s, char *format){
    char *_str;

    _str = (char *)malloc(strlen(s) + strlen(format) + 1);     
    sprintf(_str, format, s); 
    return _str;
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(int argc, char **argv){
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
}




