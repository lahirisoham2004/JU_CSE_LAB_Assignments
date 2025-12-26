%{
  /*
  Write a lex program to recognize a string that starts with a capital letter 
  which is followed by any small letter or decimal digits and ends with a special 
  character of your choice.
  */
%}
exit      [Ee][Xx][Ii][Tt]
LETTER    [A-Z]
letter    [a-z]
digit     [0-9]
special   \$
%%
{LETTER}({letter}|{digit})*{special}\n    {puts("String given follows pattern.");}
{exit}\n                                  {return 0;}
.+\n                                      {puts("String given does not follow pattern.");}
%%
inline int yywrap() {return 1;}
int main() {
  puts("Enter strings to be checked in each line. Enter EOF or \'exit\' to exit.");
  yylex();
}