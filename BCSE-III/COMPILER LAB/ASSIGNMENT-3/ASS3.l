%{
    #include <iostream>
    using namespace std;
    %}
    
    %%
    
    [1][01.]*101 { cout << "Valid binary message: " << yytext << endl; }
    .            { cout << "Invalid binary message: " << yytext << endl; }
    
    %%
    
    int main() {
        yylex();
        return 0;
    }
    
    int yywrap() {
        return 1;
    }
    