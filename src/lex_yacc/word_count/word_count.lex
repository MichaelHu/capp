%{
    int wordCount = 0;
%}

chars [A-Za-z\_\-\.]
numbers ([0-9])+
delim [ \n\t]
other [^A-Za-z\_\-\.0-9 \n\t]
whitespace {delim}+
words {chars}{2,}
singlechar {chars}
others {other}+


%%


{words} { 
    wordCount++; 
    printf(
        "[%d] %s (%d)\n"
        , wordCount
        , yytext
        , (int)yyleng
    );
}

{whitespace} {}

{numbers} {} 

{singlechar} {}

{other}  


%%

int main(int argc, char **argv) {
    FILE *f;
    int a;
    char s[100], *inputFile = NULL;

    if(argc == 2){
        inputFile = argv[1];
    }

    if((f = fopen(
        inputFile 
            ? inputFile 
            : "./word_count.lex", "r"))){
        yyin = f;

        yylex();

        printf("\nCount of words: %d\n", wordCount);
    }

    return 0;
}

int yywrap(){
    return 1;
}
