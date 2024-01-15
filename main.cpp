#include <iostream>
#include <string>
#include <vector>
using namespace std;

// variabila globala != variabila din main
// variabilele din metodele unei clase
// variabilele dintr-o functie
// variabilele din for,if,while

extern FILE *yyin;
extern char *yytext;
extern int yylineno;
extern int yylex();
extern void yyerror(const char *s);

struct Variable {
  string type;
  string name;
  bool isConstant;
};

class ScopeNode {
public:
  string name;
  ScopeNode *parent;
  std::vector<Variable> variables;
  vector<ScopeNode *> children;

  ScopeNode(string name) {
    this->name = name;
    parent = nullptr;
  }

  void addVariable(Variable variable) {
    if (existsVariable(variable.name)) {
      yyerror(string("Variable: " + variable.name +
                     " already exists in scope " + name +
                     ". You're trying to redeclare the variable")
                  .c_str());
      return;
    }

    variables.push_back(variable);
  }

  void addScopeNode(ScopeNode *node) { children.push_back(node); }

  bool existsVariable(string variableName) {
    ScopeNode *currentNode = this;

    while (currentNode != nullptr) {
      for (Variable var : currentNode->variables) {
        if (var.name == variableName) {
          return true;
        }
      }
      currentNode = currentNode->parent;
    }

    return false;
  }

  void printScopeVariables() {
    ScopeNode *currentNode = this;

    while (currentNode != nullptr) {
      for (Variable var : currentNode->variables) {
        cout << "name: " << var.name << " type: " << var.type << endl;
      }
      currentNode = currentNode->parent;
    }
  }

  void printCurrentScopeVariables() {
    for (Variable var : variables) {
      cout << "name: " << var.name << " type: " << var.type << endl;
    }
    printf("\n\n");
  }

  void printScope() {
    cout << "Scope name: " << name << endl;
    printCurrentScopeVariables();
    for (ScopeNode *node : children) {
      node->printScope();
    }
  }

  static void enterScope(string name, ScopeNode *&currentScope) {
    ScopeNode *newScope = new ScopeNode(name);
    newScope->parent = currentScope;
    currentScope->addScopeNode(newScope);
    currentScope = newScope;
  }

  static void exitScope(ScopeNode *&currentScope) {
    if (currentScope->parent != nullptr) {
      currentScope = currentScope->parent;
    }
  }
};

struct Function {
  string name;
  string returnType;
  std::vector<Variable> parameters;
  string node;
};

class VariableUtility {
public:
  std::vector<Variable> variables;

  VariableUtility() {}
  ~VariableUtility() { variables.clear(); }

  void addVar(const char *type, const char *name) {
    Variable var = {string(type), string(name)};
    variables.push_back(var);
  }

  bool existsVar(const char *var) {
    string strvar = string(var);
    for (const Variable &v : variables) {
      if (strvar == v.name) {
        return true;
      }
    }
    return false;
  }

  void printVars() {
    for (const Variable &v : variables) {
      cout << "name: " << v.name << " type:" << v.type << endl;
    }
  }
};

class ClassUtility {};

class FunctionUtility {};