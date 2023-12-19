%{
#include <stdio.h>
#include "limbaj.tab.h"
%}
%option noyywrap
%%
"intreg"|"decimal"|"caracter"|"sir"|"bit"|"vid" {yylval.string = strdup(yytext); return TYPE;}
"inceput" {return BGIN;}
"sfarsit" {return END;}
[a-zA-Z][a-zA-Z0-9]* {yylval.string = strdup(yytext); return ID;}
"=" {return ASSIGN;}
[0-9]+ {return NR;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}

 
