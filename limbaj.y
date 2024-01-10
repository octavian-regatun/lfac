%{
#include <iostream>
#include <vector>
#include "IdList.h"
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern int yylex();
void yyerror(const char * s);
class IdList ids;
%}
%union {
     char* string;
}
%token  BGIN END ASSIGN NR MULTIPLY MINUS DIVIDE MODULO AND OR EQUAL NOT_EQUAL GREATER LESS GREATER_EQUAL LESS_EQUAL POINT QUOTE_MARK PLUS LEFT_SQUARE RIGHT_SQUARE LEFT_PAREN RIGHT_PAREN FOR IF ELSE OF CLASS FUNCTION COLON LEFT_CURLY RIGHT_CURLY ARROW PUBLIC PRIVATE CONST WHILE BREAK
%token<string> ID TYPE BOOL_VALUE
%start progr
%%
progr: declarations main {printf("The programme is correct!\n\n");}
     ;

declarations :  declaration ';'          
	      |  declarations declaration ';'   
	      ;

declaration  :  ID COLON TYPE { if(!ids.existsVar($3)) {
                          ids.addVar($3,$1);
                     }
                    }
               | ID COLON TYPE ASSIGN expression
               | ID COLON TYPE LEFT_SQUARE NR RIGHT_SQUARE
               | CONST ID COLON TYPE ASSIGN expression
               | function 
               | class_definition  
               ;

expression :
    NR
  | ID
  | BOOL_VALUE
  | function_call
  | expression PLUS expression
  | expression MINUS expression
  | expression MULTIPLY expression
  | expression DIVIDE expression
  | expression EQUAL expression
  | expression NOT_EQUAL expression
  | expression GREATER expression
  | expression LESS expression
  | expression GREATER_EQUAL expression
  | expression LESS_EQUAL expression
  | expression AND expression
  | expression OR expression
  | LEFT_PAREN expression RIGHT_PAREN
  ;

/* instructions */
if:
    IF LEFT_PAREN expression RIGHT_PAREN LEFT_CURLY statements RIGHT_CURLY
    | IF LEFT_PAREN expression RIGHT_PAREN LEFT_CURLY statements RIGHT_CURLY else
    ;

else: 
      ELSE LEFT_CURLY statements RIGHT_CURLY
    | ELSE if
    ;

for : FOR ID OF ID LEFT_CURLY function_body RIGHT_CURLY
    ;

while : WHILE LEFT_PAREN expression RIGHT_PAREN LEFT_CURLY while_body RIGHT_CURLY
    ;

while_body : function_body
               | BREAK ';'
               | while_body BREAK ';'
    
function_call :
    ID LEFT_PAREN argument_list RIGHT_PAREN
    ;

argument_list :
    | expression
    | argument_list ',' expression
    ;

class_definition :
    CLASS ID LEFT_CURLY class_body RIGHT_CURLY
    ;

class_body :
    | class_body class_member ';'
    ;

class_member:   property
              | method
              ;

property: PUBLIC declaration
        | PRIVATE declaration
        ;

method: PUBLIC ID LEFT_PAREN parameters RIGHT_PAREN ARROW TYPE LEFT_CURLY function_body RIGHT_CURLY
     | PRIVATE ID LEFT_PAREN parameters RIGHT_PAREN ARROW TYPE LEFT_CURLY function_body RIGHT_CURLY
          ;

function : FUNCTION ID LEFT_PAREN parameters RIGHT_PAREN ARROW TYPE LEFT_CURLY function_body RIGHT_CURLY
         ; 

function_body : 
    | function_body declaration ';'
    | function_body statement ';'
    ;

parameters: ID COLON TYPE
          | parameters ',' ID COLON TYPE
          ;

list_param : param
            | list_param ','  param 
            ;
            
param : TYPE ID 
      ; 
      

main : BGIN main_body END  
     ;
     

main_body : 
    | main_body declaration ';'
    | main_body statement ';'
    | main_body if
    | main_body for
    | main_body while
    | main_body function_call ';'
    ;

statements :  statement ';' 
     | statements statement ';'
     ;

statement: ID ASSIGN ID
         | ID ASSIGN NR
         | ID ASSIGN BOOL_VALUE
         | ID LEFT_SQUARE NR RIGHT_SQUARE ASSIGN ID
         | ID LEFT_SQUARE NR RIGHT_SQUARE ASSIGN NR
         | ID LEFT_SQUARE NR RIGHT_SQUARE ASSIGN BOOL_VALUE
         ;
        
call_list : NR
           | call_list ',' NR
           ;
%%
void yyerror(const char * s){
     printf("error: %s at line:%d\n",s,yylineno);
}

int main(int argc, char** argv){
     yyin=fopen(argv[1],"r");
     yyparse();
     cout << "Variables:" <<endl;
     ids.printVars();
} 