%{
#include <stdio.h>
#include "limbaj.tab.h"
%}
%option noyywrap
%%
"intreg"|"decimal"|"caracter"|"sir"|"bit"|"vid"|"oare" {yylval.string = strdup(yytext); return TYPE;}
"adevarat"|"fals" {yylval.string = strdup(yytext); return BOOL_VALUE;}
"inceput" {return BGIN;}
"sfarsit" {return END;}
"*"         return MULTIPLY;
"-"         return MINUS;
"/"         return DIVIDE;
"%"         return MODULO;
"&&"        return AND;
"||"        return OR;
"=="        return EQUAL;
"!="        return NOT_EQUAL;
">"         return GREATER;
"<"         return LESS;
">="        return GREATER_EQUAL;
"<="        return LESS_EQUAL;
"."         return POINT;
"\""        return QUOTE_MARK;
"+"         return PLUS;
"["         return LEFT_SQUARE;
"]"         return RIGHT_SQUARE;
"("         return LEFT_PAREN;
")"         return RIGHT_PAREN;
"pentru"    return FOR;
"daca"      return IF;
"altfel"    return ELSE;
"din"       return OF;
"clasa"     return CLASS;
"functie"   return FUNCTION;
[a-zA-Z][a-zA-Z0-9]* {yylval.string = strdup(yytext); return ID;}
"=" {return ASSIGN;}
[0-9]+ {return NR;}
[ \t] ;
\n {yylineno++;}
. {printf("Lexical error at line %d: unexpected character '%s'\n", yylineno, yytext); return yytext[0];}

 
