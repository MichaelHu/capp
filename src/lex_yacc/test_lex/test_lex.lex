%{

%}

%x COMMENT

%%

\/\/                    { BEGIN COMMENT; printf("TWO\n"); }
<COMMENT>.              ;
<COMMENT>\n             { BEGIN INITIAL; }

\/                      printf("ONE\n");
.|\n                    ;

\/\/\/                  printf("THREE\n");
\/(\\\/|[^\n\t\/])+\/   printf("REG\n");


%%

int yywrap(){
    return 1;
}

int main(int argc, char **argv){
    yyin = fopen(argv[1], "r");
    yylex();
    fclose(yyin);
}


