%{
  /*  Write a lex file to check the validity of a binary message that starts with 1 
  and ends with 101 and contains any number of dots in between the parts of the messages. */
%}
%%

1\.*101\n             {puts("String follows pattern.");}

[Ee][Xx][Ii][Tt]\n    {return 0;}

.+\n                  {puts("String does not follow pattern.");}

%%
inline int yywrap() {return 1;}

int main() {
  puts("Enter strings to be checked in each line. Enter EOF or \'exit\' to exit.");
  yylex();
}