%{
  /*Write a lex program that will function as a calculator and perform the
   operations like addition, subtraction, multiplication, division, modulo and power.*/
  #include <stdbool.h>
  enum Operation {
    PLUS, MINUS, MUL, DIV, MOD, POW
  } op = PLUS;
  int accumulator = 0;
  bool canTerminate = false;
  void reset() {
    op = PLUS; accumulator = 0;
    canTerminate = false;
  }
  int intpow(int x, int n) {
    int y;
    if(!n) return 1;
    y = 1;
    while(n > 1) {
      if(n & 1) {
        y *= x;
        n--;
      }
      x *= x;
      n >>= 1;
    }
    return x*y;
  }
%}
digitstart                  [1-9]
digit                       [0-9]
plus                        \+
minus                       \-
mul                         \*
div                         \/
mod                         \%
pow                         \^
whitespace                  [ \t\f]
%%

{whitespace}+               {/*Ignore.*/}

{digitstart}{digit}*        {
  int result = atoi(yytext);
  switch(op) {
    case PLUS:
      accumulator += result;
      break;
    case MINUS:
      accumulator -= result;
      break;
    case MUL:
      accumulator *= result;
      break;
    case DIV:
      accumulator /= result;
      break;
    case MOD:
      accumulator %= result;
      break;
    case POW:
      accumulator = intpow(accumulator, result);
      break;
  }
  canTerminate = true;
  //printf("Recognized number %d, accumulator %d\n", result, accumulator);
}

{plus}                      {op = PLUS; canTerminate = false;}
{minus}                     {op = MINUS; canTerminate = false;}
{mul}                       {op = MUL; canTerminate = false;}
{div}                       {op = DIV; canTerminate = false;}
{mod}                       {op = MOD; canTerminate = false;}
{pow}                       {op = POW; canTerminate = false;}

\n                          {
  //puts("Calculating...");
  if(!canTerminate) {
    fputs("Illegal expression.\n", stderr);
  } else {
    printf("%d\n", accumulator);
  }
  reset();
}

.                           {
  int ch;
  fputs("Illegal character entered. Ignoring line.\n", stderr);
  while((ch = input()) != '\n' && ch != EOF);
  reset();
}
%%
inline int yywrap() {return 1;}
int main() {
  puts("Enter expression on each line. Enter EOF to exit.");
  yylex();
}