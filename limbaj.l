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
"{"         return LEFT_CURLY;
"}"         return RIGHT_CURLY;
"pentru"    return FOR;
"cat_timp"  return WHILE;
"daca"      return IF;
"altfel"    return ELSE;
"din"       return OF;
"clasa"     return CLASS;
"functie"   return FUNCTION;
":"         return COLON;
"->"        return ARROW;
"public"    return PUBLIC;
"privat"    return PRIVATE;
"constanta" return CONST;
"sparge"    return BREAK;
[a-zA-Z][a-zA-Z0-9]* {yylval.string = strdup(yytext); return ID;}
"=" {return ASSIGN;}
[0-9]+ {return NR;}
[ \t] ;
\n {yylineno++;}
"$$".*
. {return yytext[0];}
 
