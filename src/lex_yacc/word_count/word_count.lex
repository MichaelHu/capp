    /*=== definitions section  ===*/
%{
    int wordCount = 0;
%}
    /* substitution */
chars [A-Za-z\_\-\.]
numbers ([0-9])+
newline \n
delim [ {newline}\t]
other [^A-Za-z\_\-\.0-9 \n\t]
whitespace {delim}+
words {chars}{2,}
singlechar {chars}
others {other}+


%%
    /*=== rules section  ===*/


{words} { 
    wordCount++; 
    printf(
        "[%d] %s (line%d, %d chars)\n"
        , wordCount
        , yytext
        , yylineno
        , (int)yyleng
    );
}

{newline} yylineno++;

    /* do nothing use {} */
{numbers} {} 
    /* do nothing use empty */
{whitespace} 
    /* do nothing use semi-colon */
{singlechar} ; 
{other}


%%
    /*=== subroutines section  ===*/

int main(int argc, char **argv) {
    FILE *f;
    char *inputFile = NULL;

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

    /* wrapup, return 1 if done, 0 if not done */
int yywrap(){
    return 1;
}

