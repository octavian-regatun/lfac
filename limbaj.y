%{
#include <iostream>
#include <vector>
#include "main.cpp"
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern int yylex();
void yyerror(const char * s);
class VariableUtility ids;

ScopeNode *globalScope = new ScopeNode("global");
ScopeNode *currentScope = globalScope;

ScopeNode *currentClassScope = NULL;
ScopeNode *currentFunctionScope = NULL;
%}
%union {
     char* string;

    struct {
        char* type;
        char* name;
    } var_info;
}

%type <var_info> declaration

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

/* class declarations */
/* global vars */
/* global functions */
/* main */

progr: class_definitions declarations functions main {printf("The programme is correct!\n\n");}
     ;



/* progr: declarations main {printf("The programme is correct!\n\n");}
     ; */



declarations :  declaration ';'          
	      |  declarations declaration ';'   
	      ;

declaration  :  ID COLON TYPE {
    $$ = {$3,$1};  
    if(currentClassScope == NULL && currentFunctionScope== NULL){
        currentScope->addVariable(Variable{string($3),string($1),0});
    }
    else if(currentFunctionScope != NULL){
        currentFunctionScope->addVariable(Variable{string($3),string($1),0});
    }
    else if(currentClassScope != NULL){
        currentClassScope->addVariable(Variable{string($3),string($1),0});
    }
}
               | ID COLON TYPE ASSIGN expression_or_boolean {$$ = {NULL, NULL};}
               | ID COLON TYPE LEFT_SQUARE NR RIGHT_SQUARE {$$ = {NULL, NULL};}
               | CONST ID COLON TYPE ASSIGN expression_or_boolean {$$ = {NULL, NULL};}
               /* | function 
               | class_definition   */
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
if_statement :
    IF LEFT_PAREN boolean_expression RIGHT_PAREN if_compound_statement
    | IF LEFT_PAREN boolean_expression RIGHT_PAREN if_compound_statement else_statement
    ;

if_compound_statement :
    LEFT_CURLY { ScopeNode::enterScope(string("if"), currentScope); }
    function_body
    RIGHT_CURLY { ScopeNode::exitScope(currentScope); }
    ;

else_compound_statement:
    LEFT_CURLY { ScopeNode::enterScope(string("else"), currentScope); }
    function_body
    RIGHT_CURLY { ScopeNode::exitScope(currentScope); }
    ;

else_statement: 
      ELSE else_compound_statement
    | ELSE if_statement
    ;

for : FOR ID OF ID LEFT_CURLY {
          ScopeNode::enterScope("for", currentScope);
      }
      function_body {
          ScopeNode::exitScope(currentScope);
      }
      RIGHT_CURLY
    ;

while : WHILE LEFT_PAREN boolean_expression RIGHT_PAREN LEFT_CURLY {
            ScopeNode::enterScope(string("while"), currentScope);
        }
        function_body {
            ScopeNode::exitScope(currentScope);
        } RIGHT_CURLY
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
    CLASS ID LEFT_CURLY {
        currentClassScope = new ScopeNode("clasa");
    }
     class_body{
        currentClassScope = NULL;
     }
      RIGHT_CURLY
    ;

class_definitions: 
    | class_definitions class_definition
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

method: PUBLIC function
     | PRIVATE function
          ;

constructor: ID LEFT_PAREN parameters RIGHT_PAREN LEFT_CURLY function_body RIGHT_CURLY
          ;

destructor: TILDA ID LEFT_PAREN RIGHT_PAREN LEFT_CURLY function_body RIGHT_CURLY
          ;

function: FUNCTION ID LEFT_PAREN parameters RIGHT_PAREN ARROW TYPE LEFT_CURLY {currentFunctionScope=new ScopeNode("functie");} function_body {currentFunctionScope=NULL;} RIGHT_CURLY
         ; 

functions: 
    | functions function
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
      

main : BGIN {
                ScopeNode::enterScope(string("main"), currentScope);
            }
      function_body END {
                ScopeNode::exitScope(currentScope);
             }
      ;
     
function_body : 
    | function_body declaration ';'
    | function_body statement ';'
    | function_body if_statement
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
     /* cout << "Variables:" <<endl; */
     /* ids.printVars(); */
     
    globalScope->printScope();
    return 0;
        
} 