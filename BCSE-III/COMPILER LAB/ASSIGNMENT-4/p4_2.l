%{
  /*Recognize equal number of A's followed by equal number of B's*/
  long acount = 0;
  typedef enum __State {
    INITIAL, A_RUNNING, B_RUNNING, ERROR, COMPLETED
  } State;
  State current = INITIAL;
  void reset() {acount = 0; current = INITIAL;}
%}
%%

[Ee][Xx][Ii][Tt]\n              {return 0;}

A                               {
  if(current == INITIAL || current == A_RUNNING) 
    {acount++; current = A_RUNNING;}
  else current = ERROR;
}

B                               {
  if(current == A_RUNNING || current == B_RUNNING) {
    acount--; 
    if(!acount) current = COMPLETED;
    else if(acount > 0) current = B_RUNNING;
    else current = ERROR;
  }
  else current = ERROR;
}

.                               {current = ERROR;}

\n                              {
  if(current == INITIAL || current == COMPLETED)
    puts("String follows pattern.");
  else
    puts("String does not follow pattern.");
  reset();
}

%%
int yywrap() {return 1;}

int main() {
  puts("Enter strings to be checked in each line. Enter EOF or \'exit\' to exit.");
  yylex();
}