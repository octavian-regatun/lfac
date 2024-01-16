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

struct Variable
{
  string type;
  string name;
  int size;
  vector<string> value;
  bool isConstant;

  Variable(const string &t, const string &n, const int &s, const string &v, const bool &isConst)
      : type(t), name(n), size(s), isConstant(isConst)
  {
    string va = (t == "sir" && v == "0") ? "" : v;
    for (int i = 0; i < s; i++)
      value.push_back(va);
  }
};

class ScopeNode
{
public:
  string name;
  ScopeNode *parent;
  std::vector<Variable> variables;
  vector<ScopeNode *> children;

  ScopeNode(string name)
  {
    this->name = name;
    parent = nullptr;
  }

  void addVariable(Variable &variable)
  {
    if (existsVariable(variable.name))
    {
      yyerror(string("Variable: " + variable.name +
                     " already exists in scope " + name +
                     ". You're trying to redeclare the variable")
                  .c_str());
      return;
    }
    if (checkMatchingTypeAndUpdate(variable, "0", variable.value[0], false))
      variables.push_back(variable);
  }

  void addScopeNode(ScopeNode *node) { children.push_back(node); }

  bool existsVariable(string variableName)
  {
    ScopeNode *currentNode = this;

    while (currentNode != nullptr)
    {
      for (Variable var : currentNode->variables)
      {
        if (var.name == variableName)
        {
          return true;
        }
      }
      currentNode = currentNode->parent;
    }

    return false;
  }

  bool checkMatchingTypeAndUpdate(Variable &var, const std::string &index, const std::string &value, bool constant)
  {
    if (constant == true)
    {
      yyerror(string("Can't modify a constant variable").c_str());
      return false;
    }
    int idx = stoi(index);
    if (idx < 0 || idx >= var.size)
    {
      yyerror(string("Index out of bounds for variable " + var.name).c_str());
      return false;
    }

    if (var.type == "decimal")
    {
      try
      {
        std::stof(value);
        var.value[idx] = value;
        return true;
      }
      catch (const std::invalid_argument &)
      {
        yyerror(string("Type mismatch: Expected decimal for variable " + var.name + " at index " + index).c_str());
        return false;
      }
    }
    else if (var.type == "intreg")
    {
      try
      {
        cout << "\n\nBEFORE: " << var.value[idx];
        std::stoi(value);
        var.value[idx] = value;
        cout << " AFTER: " << var.value[idx];
        return true;
      }
      catch (const std::invalid_argument &)
      {
        yyerror(string("Type mismatch: Expected intreg for variable " + var.name + " at index " + index).c_str());
        return false;
      }
    }
    else if (var.type == "bit")
    {
      if (value != "true" && value != "false")
      {
        yyerror(string("Type mismatch: Expected bit for variable " + var.name + " at index " + index).c_str());
        return false;
      }
      else
      {
        var.value[idx] = (value == "true") ? "1" : "0";
        return true;
      }
    }
    else // sir
    {
      var.value[idx] = value;
      return true;
    }
  }

  void updateVariable(const string &id, const string &index, const string &value)
  {
    if (!existsVariable(id))
    {
      yyerror(string("Variable: " + id +
                     " doesn't exists in scope " + name +
                     ". You're trying to update the variable")
                  .c_str());
      return;
    }

    ScopeNode *currentNode = this;

    while (currentNode != nullptr)
    {
      for (Variable &var : currentNode->variables)
        if (var.name == id)
        {
          if (var.size == 0 && stoi(index) != 0)
          {
            yyerror(string("Variable: " + id + " isn't a vector. You're trying to update the variable").c_str());
            return;
          }

          bool smth = checkMatchingTypeAndUpdate(var, index, value, var.isConstant);
        }
      currentNode = currentNode->parent;
    }
  }
  // Printing

  void printScopeVariables()
  {
    ScopeNode *currentNode = this;

    while (currentNode != nullptr)
    {
      for (Variable var : currentNode->variables)
      {
        cout << "name: " << var.name << " type: " << var.type << endl;
      }
      currentNode = currentNode->parent;
    }
  }

  void printCurrentScopeVariables()
  {
    for (Variable var : variables)
    {
      cout << "name: " << var.name << " type: " << var.type << endl;
    }
    printf("\n\n");
  }

  void printScope()
  {
    cout << "Scope name: " << name << endl;
    printCurrentScopeVariables();
    for (ScopeNode *node : children)
    {
      node->printScope();
    }
  }

  void printVariables() const
  {
    for (const auto &var : variables)
    {
      cout << "Type: " << var.type << ", Name: " << var.name << ", Size: " << var.size
           << ", Is Constant: " << (var.isConstant ? "true" : "false") << ", Value: ";
      for (int i = 0; i < var.size; i++)
        cout << var.value[i] << " ";
      cout << "\n";
    }
  }

  void printTree(int depth = 0) const
  {
    for (int i = 0; i < depth; ++i)
    {
      std::cout << "  ";
    }
    std::cout << "\nScope: " << name << "\n";

    printVariables();

    for (const auto &child : children)
    {
      child->printTree(depth + 1);
    }
  }

  static void enterScope(string name, ScopeNode *&currentScope)
  {
    ScopeNode *newScope = new ScopeNode(name);
    newScope->parent = currentScope;
    currentScope->addScopeNode(newScope);
    currentScope = newScope;
  }

  static void exitScope(ScopeNode *&currentScope)
  {
    if (currentScope->parent != nullptr)
    {
      currentScope = currentScope->parent;
    }
  }
};

class ClassUtility
{
public:
  vector<string> classes;

  ClassUtility() {}
  void AddClass(string c)
  {
    if (ClassExists(c))
    {
      yyerror(string("Class: " + c + " already exists. You're trying to redeclare it").c_str());
      return;
    }
    classes.push_back(c);
  }
  bool ClassExists(string name)
  {
    for (auto c : classes)
      if (c == name)
        return 1;
    return 0;
  }

  void PrintClasses()
  {
    cout << "\n\nDeclared classes: ";
    for (int i = 0; i < classes.size(); i++)
      cout << classes[i] << " ";
    cout << endl;
  }
};

struct Function
{
  string name;
  string returnType;
  std::vector<Variable> parameters;
  string node;

  Function(const string &n, const string &r) : name(n), returnType(r) {}
};

class FunctionUtility
{
public:
  vector<Function> functions;

  FunctionUtility() {}
  void AddFunction(Function f)
  {
    if (FunctionExists(f.name))
    {
      yyerror(string("Function: " + f.name + " already exists. You're trying to redeclare it").c_str());
      return;
    }
    functions.push_back(f);
  }
  bool FunctionExists(string name)
  {
    for (auto f : functions)
      if (f.name == name)
        return 1;
    return 0;
  }

  void PrintFunctions()
  {
    cout << "\n\nDeclared functions: ";
    for (int i = 0; i < functions.size(); i++)
      cout << functions[i].name << " ";
    cout << endl;
  }
};

/*
class VariableUtility
{
public:
  std::vector<Variable> variables;

  VariableUtility() {}
  ~VariableUtility() { variables.clear(); }


  void addVar(const char *type, const char *name) {
    Variable var = {string(type), string(name)};
    variables.push_back(var);
  }


  bool existsVar(const char *var)
  {
    string strvar = string(var);
    for (const Variable &v : variables)
    {
      if (strvar == v.name)
      {
        return true;
      }
    }
    return false;
  }

  void printVars()
  {
    for (const Variable &v : variables)
    {
      cout << "name: " << v.name << " type:" << v.type << endl;
    }
  }
};
*/
