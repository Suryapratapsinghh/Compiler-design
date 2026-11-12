%{
#include <stdio.h>

int yylex();
void yyerror(const char *s);
%}

%token NUMBER

%%

expr:
      expr '+' expr
    | expr '*' expr
    | '(' expr ')'
    | NUMBER
    ;

%%

int main()
{
    printf("Enter an expression: ");

    if (yyparse() == 0)
        printf("String belongs to the grammar.\n");
    else
        printf("String does not belong to the grammar.\n");

    return 0;
}

void yyerror(const char *s)
{
    printf("Syntax Error\n");
}