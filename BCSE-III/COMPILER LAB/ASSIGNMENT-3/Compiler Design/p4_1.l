%{
  /*Recognize any number of A's followed by any number of B's*/
%}
%%

A*B*\n                          {puts("String follows pattern.");}
[Ee][Xx][Ii][Tt]\n              {return 0;}
.+\n                            {puts("String does not follow pattern.");}

%%
int yywrap() {return 1;}

int main() {
  puts("Enter strings to be checked in each line. Enter EOF or \'exit\' to exit.");
  yylex();
}