%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* prototypes */
int yylex(void);
void yyerror(char *s);
char *str_concat(char *s1, char *s2);
FILE *yyin;

%}

%union{
    char *text;        /* symbol table index */
};

%token <text> T K
%type <text> p s e

%nonassoc T K
%nonassoc KX

%%

p:
    s                   { printf("<%s>\n", $1); }
    ;

s:
    s e                 { $$ = str_concat($1, $2); }
    | e                 { $$ = $1; }
    ;

e:
    T                   { $$ = $1; }
    | K s K %prec KX    { $$ = str_concat($1, str_concat($2, $3)); }
    ;


%%

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
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
