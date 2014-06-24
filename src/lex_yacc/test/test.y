%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* prototypes */
int yylex(void);
int yylineno;
void yyerror(char *s);
char *str_concat(char *s1, char *s2);
FILE *yyin;

%}

%union{
    char *text;        /* symbol table index */
};

%token <text> T E
%type <text> p s e

%nonassoc T E P error
%nonassoc EX PX

%%

p:
    s                   { printf("%s\n", $1); }
    ;

s:
    s e                 { $$ = str_concat($1, $2); }
    | e                 { $$ = $1; }
    ;

e:
    T                   { $$ = $1; }
    | E s E %prec EX    { $$ = str_concat("<em>", str_concat($2, "</em>")); }
    | P s P %prec PX    { $$ = str_concat("<strong>", str_concat($2, "</strong>\n")); }
    | error '\n'        { $$ = ""; }
    ;


%%

void yyerror(char *s) {
    fprintf(stdout, "line %d: %s\n", yylineno, s);
}

char *str_concat(char *s1, char *s2){
    char *_str;

    _str = (char *)malloc(strlen(s1) + strlen(s2) + 1);     
    sprintf(_str, "%s%s", s1, s2); 
    return _str;
}

int main(int argc, char **argv){
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
}
