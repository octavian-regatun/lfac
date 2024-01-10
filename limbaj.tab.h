/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_LIMBAJ_TAB_H_INCLUDED
# define YY_YY_LIMBAJ_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    BGIN = 258,                    /* BGIN  */
    END = 259,                     /* END  */
    ASSIGN = 260,                  /* ASSIGN  */
    NR = 261,                      /* NR  */
    MULTIPLY = 262,                /* MULTIPLY  */
    MINUS = 263,                   /* MINUS  */
    DIVIDE = 264,                  /* DIVIDE  */
    MODULO = 265,                  /* MODULO  */
    AND = 266,                     /* AND  */
    OR = 267,                      /* OR  */
    EQUAL = 268,                   /* EQUAL  */
    NOT_EQUAL = 269,               /* NOT_EQUAL  */
    GREATER = 270,                 /* GREATER  */
    LESS = 271,                    /* LESS  */
    GREATER_EQUAL = 272,           /* GREATER_EQUAL  */
    LESS_EQUAL = 273,              /* LESS_EQUAL  */
    POINT = 274,                   /* POINT  */
    QUOTE_MARK = 275,              /* QUOTE_MARK  */
    PLUS = 276,                    /* PLUS  */
    LEFT_SQUARE = 277,             /* LEFT_SQUARE  */
    RIGHT_SQUARE = 278,            /* RIGHT_SQUARE  */
    LEFT_PAREN = 279,              /* LEFT_PAREN  */
    RIGHT_PAREN = 280,             /* RIGHT_PAREN  */
    FOR = 281,                     /* FOR  */
    IF = 282,                      /* IF  */
    ELSE = 283,                    /* ELSE  */
    OF = 284,                      /* OF  */
    CLASS = 285,                   /* CLASS  */
    FUNCTION = 286,                /* FUNCTION  */
    ID = 287,                      /* ID  */
    TYPE = 288,                    /* TYPE  */
    BOOL_VALUE = 289               /* BOOL_VALUE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 12 "limbaj.y"

     char* string;

#line 102 "limbaj.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_LIMBAJ_TAB_H_INCLUDED  */
