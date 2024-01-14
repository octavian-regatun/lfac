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
%token  BGIN END ASSIGN NR MULTIPLY MINUS DIVIDE MODULO AND OR EQUAL NOT_EQUAL GREATER LESS GREATER_EQUAL LESS_EQUAL POINT QUOTE_MARK PLUS LEFT_SQUARE RIGHT_SQUARE LEFT_PAREN RIGHT_PAREN FOR IF ELSE OF CLASS FUNCTION COLON LEFT_CURLY RIGHT_CURLY ARROW TILDA PUBLIC PRIVATE CONST WHILE BREAK THIS
%token<string> ID TYPE BOOL_VALUE

%left PLUS MINUS
%left MULTIPLY DIVIDE
%left UNARY_MINUS
%left AND OR
%left EQUAL NOT_EQUAL GREATER LESS GREATER_EQUAL LESS_EQUAL
%left LEFT_SQUARE RIGHT_SQUARE
%left LEFT_PAREN RIGHT_PAREN

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
               | ID COLON TYPE ASSIGN expression_or_boolean
               | ID COLON TYPE LEFT_SQUARE NR RIGHT_SQUARE
               | CONST ID COLON TYPE ASSIGN expression_or_boolean
               | function 
               | class_definition  
               ;

expression_or_boolean : expression
                     | boolean_expression
                     ;

expression : NR
           | ID
           | function_call
           | expression PLUS expression
           | expression MINUS expression
           | expression MULTIPLY expression
           | expression DIVIDE expression
           | LEFT_PAREN expression RIGHT_PAREN
           | ID LEFT_SQUARE NR RIGHT_SQUARE
           ;

boolean_expression : BOOL_VALUE
                 | expression EQUAL expression
                 | expression NOT_EQUAL expression
                 | expression GREATER expression
                 | expression LESS expression
                 | expression GREATER_EQUAL expression
                 | expression LESS_EQUAL expression
                 | expression AND expression
                 | expression OR expression
                 ;


/* instructions */
if:
    IF LEFT_PAREN boolean_expression RIGHT_PAREN LEFT_CURLY function_body RIGHT_CURLY
    | IF LEFT_PAREN boolean_expression RIGHT_PAREN LEFT_CURLY function_body RIGHT_CURLY else
    ;

else: 
      ELSE LEFT_CURLY function_body RIGHT_CURLY
    | ELSE if
    ;

for : FOR ID OF ID LEFT_CURLY function_body RIGHT_CURLY
    ;

while : WHILE LEFT_PAREN boolean_expression RIGHT_PAREN LEFT_CURLY function_body RIGHT_CURLY
    ;

/* Isn't it wrong to have the break inside the while loop? Shouldn't it be in any function?
while_body : function_body
           | BREAK ';'
           | while_body BREAK ';'
           ;
*/
    
function_call :
    ID LEFT_PAREN argument_list RIGHT_PAREN
    ;

argument_list :
    | expression_or_boolean
    | argument_list ',' expression_or_boolean
    ;

class_definition :
    CLASS ID LEFT_CURLY class_body RIGHT_CURLY
    ;

class_body :
    | class_body class_member ';'
    ;

class_member:   property
              | method
              | constructor
              | destructor
              ;

property: PUBLIC declaration
        | PRIVATE declaration
        ;

method: PUBLIC ID LEFT_PAREN parameters RIGHT_PAREN ARROW TYPE LEFT_CURLY function_body RIGHT_CURLY
     | PRIVATE ID LEFT_PAREN parameters RIGHT_PAREN ARROW TYPE LEFT_CURLY function_body RIGHT_CURLY
          ;

constructor: ID LEFT_PAREN parameters RIGHT_PAREN LEFT_CURLY function_body RIGHT_CURLY
          ;

destructor: TILDA ID LEFT_PAREN RIGHT_PAREN LEFT_CURLY function_body RIGHT_CURLY
          ;

function: FUNCTION ID LEFT_PAREN parameters RIGHT_PAREN ARROW TYPE LEFT_CURLY function_body RIGHT_CURLY
         ; 

parameters:
          | ',' ID COLON TYPE parameters
          | ID COLON TYPE parameters
          ;

/* // Seems useless for our program
list_param : param
            | list_param ','  param 
            ;
            
param : TYPE ID 
      ; 
*/
      

main : BGIN function_body END  
     ;
     
function_body : 
    | function_body declaration ';'
    | function_body statement ';'
    | function_body if
    | function_body for
    | function_body while
    | function_body function_call ';'
    | function_body BREAK ';' // How will the break determine what to "sparge" tho?
    ;

/* // When is this used? The statements are already recursive from function_body
statements :  statement ';' 
     | statements statement ';'
     ;
*/

statement:
         | ID ASSIGN expression_or_boolean
         | ID LEFT_SQUARE expression_or_boolean RIGHT_SQUARE ASSIGN expression_or_boolean
         | THIS POINT ID ASSIGN expression_or_boolean
         ;
/* // // Seems useless for our program      
 all_list : NR
           | call_list ',' NR
           ;
*/
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