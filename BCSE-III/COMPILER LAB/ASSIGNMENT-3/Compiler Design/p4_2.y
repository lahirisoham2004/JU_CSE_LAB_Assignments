%{
  #include <stdio.h>
  int yylex() {return getchar();}
  void yyerror(const char *s) {printf("Parser: %s\n", s);}
%}

%start start /*Notify start symbol to YACC*/

%%

start:
  /*epsilon*/
  | line start
  ;

line:
  expr '\n' {puts("String follows pattern.");}
  | error '\n' {puts("String does not follow pattern."); yyerrok;}
  ;

expr: 'A' expr 'B' | /*epsilon*/;

%%

int main() {
  puts("Enter strings to be checked in each line. Enter EOF to exit.");
  yyparse();
}