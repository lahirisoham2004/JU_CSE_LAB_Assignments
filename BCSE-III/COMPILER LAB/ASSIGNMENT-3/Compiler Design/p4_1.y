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

expr: a_expr b_expr;
a_expr: 'A' a_expr | /*epsilon*/;
b_expr: 'B' b_expr | /*epsilon*/;

%%

int main() {
  puts("Enter strings to be checked in each line. Enter EOF to exit.");
  yyparse();
}