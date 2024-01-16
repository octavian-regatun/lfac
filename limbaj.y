%{
#include <iostream>
#include <cstring>
#include <vector>
#include "main.cpp"
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern int yylex();
void yyerror(const char * s);
//class VariableUtility ids;

ScopeNode *globalScope = new ScopeNode("global");
ScopeNode *currentScope = globalScope;

ScopeNode *currentClassScope = NULL;
ScopeNode *currentFunctionScope = NULL;

FunctionUtility *functions = new FunctionUtility();
ClassUtility *classes = new ClassUtility();

void addVariableToScope(Variable var, ScopeNode *currentScope, ScopeNode *currentClassScope, ScopeNode *currentFunctionScope)
{
  if(currentClassScope == NULL && currentFunctionScope== NULL){
        currentScope->addVariable(var);
    }
    else if(currentFunctionScope != NULL){
        currentFunctionScope->addVariable(var);
    }
    else if(currentClassScope != NULL){
        currentClassScope->addVariable(var);
    }
}

void updateScopeVariable(const string& id, const string& index, const string& value, ScopeNode *currentScope, ScopeNode *currentClassScope, ScopeNode *currentFunctionScope)
{
    if(currentClassScope == NULL && currentFunctionScope== NULL){
        currentScope->updateVariable(id, index, value);
    }
    else if(currentFunctionScope != NULL){
        currentFunctionScope->updateVariable(id, index, value);
    }
    else if(currentClassScope != NULL){
        currentClassScope->updateVariable(id, index, value);
    }
}

%}
%union {
     char* string;

    struct {
        char* type;
        char* name;
        char* value;
    } var_info;
}

%type <var_info> declaration
%type <string> expression_or_boolean expression boolean_expression function_call NR LEFT_PAREN

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

progr: class_definitions declarations functions main {printf("\n\nThe program is syntactically correct! Any logic errors, if present, can be found above.\n\n");}
     ;


declarations :  declaration ';'          
	      |  declarations declaration ';'   
	      ;

declaration: ID COLON TYPE { $$ = {$3,$1,strdup("")};
    addVariableToScope(Variable{string($3), string($1), 1, "0", 0}, currentScope, currentClassScope, currentFunctionScope);}
    
    | ID COLON TYPE ASSIGN expression_or_boolean {$$ = {$3,$1,$5};
    addVariableToScope(Variable{string($3), string($1), 1, string($5), 0}, currentScope, currentClassScope, currentFunctionScope);}
    
    | ID COLON TYPE LEFT_SQUARE NR RIGHT_SQUARE {$$ = {$3,$1,$5};
    addVariableToScope(Variable{string($3), string($1), stoi($5), "0", 0}, currentScope, currentClassScope, currentFunctionScope);
    }
    
    | CONST ID COLON TYPE ASSIGN expression_or_boolean {$$ = {$4,$2,$6};
    addVariableToScope(Variable{string($4), string($2), 1, string($6), 1}, currentScope, currentClassScope, currentFunctionScope);}
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
           | ID LEFT_SQUARE NR RIGHT_SQUARE {$$ = strdup(yytext);}
           ;

boolean_expression : BOOL_VALUE
                 | expression EQUAL expression
                 | expression NOT_EQUAL expression
                 | expression GREATER expression
                 | expression LESS expression
                 | expression GREATER_EQUAL expression
                 | expression LESS_EQUAL expression
                 | expression AND expression
                 | expression OR expression {$$ = strdup(yytext);}
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
    
function_call :
    ID LEFT_PAREN argument_list RIGHT_PAREN {$$ = strdup(yytext);}
    ;

argument_list :
    | expression_or_boolean
    | argument_list ',' expression_or_boolean
    ;

class_definition :
    CLASS ID LEFT_CURLY {
        currentClassScope = new ScopeNode("clasa");
        globalScope->addScopeNode(currentClassScope); // append class node
        classes->AddClass(string($2));
    }
     class_body{
        currentClassScope = NULL;
        currentScope = globalScope; // go back to the root if necessary
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

function: FUNCTION ID LEFT_PAREN parameters RIGHT_PAREN ARROW TYPE LEFT_CURLY 
        {currentFunctionScope=new ScopeNode("functie");
        globalScope->addScopeNode(currentFunctionScope);
        functions->AddFunction(Function{string($2), string($7)});
        } function_body 
        {currentFunctionScope=NULL;
        currentScope = globalScope;} RIGHT_CURLY
         ; 

functions: 
    | functions function
    ;

parameters:
          | ',' ID COLON TYPE parameters
          | ID COLON TYPE parameters
          ;

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
    | function_body BREAK ';'
    ;

statement:
    | ID ASSIGN expression_or_boolean {
        updateScopeVariable($1, "0", $3, currentScope, currentClassScope, currentFunctionScope);
    }
    | ID LEFT_SQUARE expression_or_boolean RIGHT_SQUARE ASSIGN expression_or_boolean {
        updateScopeVariable($1, $3, $6, currentScope, currentClassScope, currentFunctionScope);
    }
    | THIS POINT ID ASSIGN expression_or_boolean {
       // if (checkIfInClass()) 
            updateScopeVariable($3, "0", $5, currentScope, currentClassScope, currentFunctionScope);
    }
    ;


%%

void yyerror(const char * s){
     printf("error: %s at line:%d\n",s,yylineno);
}

int main(int argc, char** argv){
    yyin=fopen(argv[1],"r");
    yyparse();
     
    // globalScope->printScope();
    globalScope->printTree();
    functions->PrintFunctions();
    classes->PrintClasses();
    return 0;
        
} 