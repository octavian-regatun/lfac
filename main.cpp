#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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

  Variable(const string &t, const string &n, const int &s, const string &v,
           const bool &isConst)
      : type(t), name(n), size(s), isConstant(isConst)
  {
    string va = (t == "sir" && v == "0") ? "" : v;
    for (int i = 0; i < s; i++)
      value.push_back(va);
  }

  void setValue(char *val) { value[0] = string(val); }
  void print()
  {
    cout << "Type: " << type << ", Name: " << name << ", Size: " << size
         << ", Is Constant: " << (isConstant ? "true" : "false") << ", Value: ";
    for (int i = 0; i < size; i++)
      cout << value[i] << " ";
    cout << "\n";
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

  bool checkMatchingTypeAndUpdate(Variable &var, const std::string &index,
                                  const std::string &value, bool constant)
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
        yyerror(string("Type mismatch: Expected decimal for variable " +
                       var.name + " at index " + index)
                    .c_str());
        return false;
      }
    }
    else if (var.type == "intreg")
    {
      try
      {
        std::stoi(value);
        var.value[idx] = value;
        return true;
      }
      catch (const std::invalid_argument &)
      {
        yyerror(string("Type mismatch: Expected intreg for variable " +
                       var.name + " at index " + index)
                    .c_str());
        return false;
      }
    }
    else if (var.type == "bit")
    {
      if (value != "true" && value != "false")
      {
        yyerror(string("Type mismatch: Expected bit for variable " + var.name +
                       " at index " + index)
                    .c_str());
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

  void updateVariable(const string &id, const string &index,
                      const string &value)
  {
    if (!existsVariable(id))
    {
      yyerror(string("Variable: " + id + " doesn't exists in scope " + name +
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
            yyerror(
                string("Variable: " + id +
                       " isn't a vector. You're trying to update the variable")
                    .c_str());
            return;
          }

          bool smth =
              checkMatchingTypeAndUpdate(var, index, value, var.isConstant);
        }
      currentNode = currentNode->parent;
    }
  }

  Variable findVariable(const string &id)
  {
    if (!existsVariable(id))
    {
      yyerror(string("Variable: " + id + " doesn't exists in scope " + name +
                     ". You're trying to find the variable")
                  .c_str());
      return Variable("", "", 0, "", false);
    }

    ScopeNode *currentNode = this;

    while (currentNode != nullptr)
    {
      for (Variable &var : currentNode->variables)
        if (var.name == id)
        {
          return var;
        }
      currentNode = currentNode->parent;
    }

    return Variable("", "", 0, "", false);
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
      cout << "Type: " << var.type << ", Name: " << var.name
           << ", Size: " << var.size
           << ", Is Constant: " << (var.isConstant ? "true" : "false")
           << ", Value: ";
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

  // Go back to class node when using smth like: class.variable = x
  ScopeNode *findScopeByName(const string &targetName)
  {
    if (name == targetName)
    {
      return this;
    }

    for (ScopeNode *child : children)
    {
      ScopeNode *foundScope = child->findScopeByName(targetName);
      if (foundScope != nullptr)
      {
        return foundScope;
      }
    }

    return nullptr;
  }

  static void setCurrentClassScopeByName(const string &targetName, ScopeNode *globalScope, ScopeNode *&currentClassScope)
  {
    ScopeNode *targetScope = globalScope->findScopeByName(targetName);
    if (targetScope != nullptr)
    {
      currentClassScope = targetScope;
    }
    else
    {
      cout << "Scope with name " << targetName << " not found.\n";
    }
  }
};

enum Visibility
{
  PUBLIC_KEYWORD,
  PRIVATE_KEYWORD
};

struct ClassVariables
{
  string name;
  Visibility visible;
};

struct ClassFunctions
{
  string name;
  Visibility visible;
};

struct SingularClass
{
  string name;
  vector<ClassVariables> variables;
  vector<ClassFunctions> functions;

  SingularClass(const string &n) : name(n) {}
};

class ClassUtility
{
public:
  vector<SingularClass> classes;

  ClassUtility() {}

  void AddClass(string c)
  {
    if (ClassExists(c))
    {
      cerr << "Class: " << c << " already exists. You're trying to redeclare it" << endl;
      return;
    }
    classes.push_back(SingularClass(c));
  }

  void AddVariableToCurrentClass(string variableName, bool visibility)
  {
    if (!classes.empty())
    {
      if (!VariableExistsInCurrentClass(variableName))
      {
        classes.back().variables.push_back({variableName, static_cast<Visibility>(visibility)});
      }
      else
      {
        cerr << "Variable: " << variableName << " already exists in the current class." << endl;
      }
    }
    else
    {
      cerr << "Error: No class available to add variables." << endl;
    }
  }

  void AddFunctionToCurrentClass(string functionName, bool visibility)
  {
    if (!classes.empty())
    {
      if (!FunctionExistsInCurrentClass(functionName))
      {
        classes.back().functions.push_back({functionName, static_cast<Visibility>(visibility)});
      }
      else
      {
        cerr << "Function: " << functionName << " already exists in the current class." << endl;
      }
    }
    else
    {
      cerr << "Error: No class available to add functions." << endl;
    }
  }

  bool ClassExists(string name)
  {
    for (auto &c : classes)
      if (c.name == name)
        return true;
    return false;
  }

  bool VariableExistsInCurrentClass(string name)
  {
    if (!classes.empty())
    {
      for (const auto &variable : classes.back().variables)
      {
        if (variable.name == name)
        {
          return true;
        }
      }
    }
    return false;
  }

  bool FunctionExistsInCurrentClass(string name)
  {
    if (!classes.empty())
    {
      for (const auto &function : classes.back().functions)
      {
        if (function.name == name)
        {
          return true;
        }
      }
    }
    return false;
  }

  void PrintClasses()
  {
    cout << "\n\nDeclared classes: ";
    for (const auto &c : classes)
    {
      cout << c.name << " [";
      for (size_t i = 0; i < c.variables.size(); i++)
      {
        cout << (c.variables[i].visible == PUBLIC_KEYWORD ? "public" : "private") << " variable " << c.variables[i].name;
        if (i < c.variables.size() - 1)
        {
          cout << ", ";
        }
      }

      if (!c.variables.empty() && !c.functions.empty())
      {
        cout << ", ";
      }

      for (size_t i = 0; i < c.functions.size(); i++)
      {
        cout << (c.functions[i].visible == PUBLIC_KEYWORD ? "public" : "private") << " function " << c.functions[i].name;
        if (i < c.functions.size() - 1)
        {
          cout << ", ";
        }
      }

      cout << "] ";
    }
    cout << endl;
  }

  bool checkIfPrivate(const string &className, const string &variableName)
  {
    for (const auto &c : classes)
    {
      if (c.name == className)
      {
        for (const auto &variable : c.variables)
        {
          if (variable.name == variableName && variable.visible == Visibility::PRIVATE_KEYWORD)
            return true;
        }
        yyerror("Variable doesn't exist in class. You're trying a redeclaration");
        return false;
      }
    }
    yyerror("Class doesn't exists. Called");
    return false;
  }
};

struct Function
{
  string name;
  string returnType;
  vector<Variable> parameters;
  vector<Variable> queue;
  string node;

  Function(const string &n, const string &node) : name(n), node(node) {}

  void setReturnType(char *type) { returnType = string(type); }

  void printQueue()
  {
    cout << "Queue:\n";
    for (auto &var : queue)
    {
      cout << "Type: " << var.type << ", Name: " << var.name
           << ", Size: " << var.size
           << ", Is Constant: " << (var.isConstant ? "true" : "false")
           << ", Value: ";
      for (int i = 0; i < var.size; i++)
        cout << var.value[i] << " ";
      cout << "\n";
    }
  }

  bool existsVariableInQueue(const string &id)
  {
    for (Variable &var : queue)
      if (var.name == id)
      {
        return true;
      }
    return false;
  }

  Variable findLastVariableInQueue(const string &id)
  {
    for (int i = queue.size() - 1; i >= 0; i--)
      if (queue[i].name == id)
      {
        return queue[i];
      }
    return Variable("", "", 0, "", false);
  }
};

class FunctionUtility
{
public:
  vector<Function *> functions;

  FunctionUtility() {}
  void AddFunction(Function *f)
  {
    if (FunctionExists(f->name))
    {
      yyerror(string("Function: " + f->name +
                     " already exists. You're trying to redeclare it")
                  .c_str());
      return;
    }
    functions.push_back(f);
  }
  bool FunctionExists(string name)
  {
    for (auto f : functions)
      if (f->name == name)
        return 1;
    return 0;
  }

  void PrintFunctions()
  {
    cout << "\n\nDeclared functions:\n";
    for (auto &function : functions)
    {
      cout << "Nume:" << function->name
           << " Return type: " << function->returnType
           << " Node: " << function->node << endl;
      for (int j = 0; j < function->parameters.size(); j++)
      {
        cout << "Parametru: " << function->parameters[j].name << " "
             << function->parameters[j].type << endl;
      }
    }
    cout << endl;
  }
};

class AST
{
public:
  string value;
  string type;
  AST *left;
  AST *right;
  vector<string> booleanOperators{"&&", "||"};
  vector<string> numberOperators{"+", "-", "*", "/", "==", "!=", ">", "<", ">=", "<="};

  AST(const string &v = "", const string &t = "", AST *leftChild = nullptr, AST *rightChild = nullptr)
      : value(v), type(t), left(leftChild), right(rightChild)
  {
  }

  ~AST()
  {
    if (left != nullptr)
    {
      delete left;
      left = nullptr;
    }

    if (right != nullptr)
    {
      delete right;
      right = nullptr;
    }
  }

  void addChild(AST *leftChild, string root = "", AST *rightChild = nullptr)
  {
    if (root == "")
    {
      left = leftChild;
      left->type = type;
    }
    else
    {
      left = leftChild;
      value = root;
      right = rightChild;
      left->type = type;
      if (right != nullptr)
        right->type = type;
    }
  }

  string evaluate()
  {
    if (isalpha(value[0]))
      return left->value;
    if (left == nullptr && right == nullptr)
    {
      return value;
    }
    if (type == "sir" || type == "vid" || type == "caracter")
    {
      yyerror("We don't support operators for this type of variable");
      return "";
    }

    string leftValue = left ? left->evaluate() : "";
    string rightValue = right ? right->evaluate() : "";

    if (std::find(booleanOperators.begin(), booleanOperators.end(), value) != booleanOperators.end())
    {
      if ((leftValue != "0" && leftValue != "1") || (rightValue != "0" && rightValue != "1"))
        yyerror("Can't use this operator");
      else
      {
        bool convertedLeftValue = (bool)stoi(leftValue);
        bool convertedRightValue = (bool)stoi(rightValue);
        if (value == "&&")
          return to_string(convertedLeftValue && convertedRightValue);
        else if (value == "||")
          return to_string(convertedLeftValue || convertedRightValue);
      }
    }
    else if (std::find(numberOperators.begin(), numberOperators.end(), value) != numberOperators.end())
    {
      if (type == "oare")
      {
        yyerror("Incompatible operations");
        return "";
      }
      if (type == "intreg")
      {
        int convertedLeftValue, convertedRightValue;
        try
        {
          convertedLeftValue = std::stoi(leftValue);
          convertedRightValue = std::stoi(rightValue);
        }
        catch (const std::invalid_argument &)
        {
          yyerror("Invalid argument for integer conversion");
          return "";
        }
        catch (const std::out_of_range &)
        {
          yyerror("Out of range for integer conversion");
          return "";
        }

        if (value == "+")
          return to_string(convertedLeftValue + convertedRightValue);
        else if (value == "-")
          return to_string(convertedLeftValue - convertedRightValue);
        else if (value == "*")
          return to_string(convertedLeftValue * convertedRightValue);
        else if (value == "/")
          return to_string(convertedLeftValue / convertedRightValue);
        else if (value == "==")
          return to_string(convertedLeftValue == convertedRightValue);
        else if (value == "!=")
          return to_string(convertedLeftValue != convertedRightValue);
        else if (value == ">")
          return to_string(convertedLeftValue > convertedRightValue);
        else if (value == "<")
          return to_string(convertedLeftValue < convertedRightValue);
        else if (value == ">=")
          return to_string(convertedLeftValue >= convertedRightValue);
        else if (value == "<=")
          return to_string(convertedLeftValue <= convertedRightValue);
      }
      else if (type == "decimal")
      {
        float convertedLeftValue, convertedRightValue;
        try
        {
          convertedLeftValue = std::stof(leftValue);
          convertedRightValue = std::stof(rightValue);
        }
        catch (const std::invalid_argument &)
        {
          yyerror("Invalid argument for float conversion");
          return "";
        }
        catch (const std::out_of_range &)
        {
          yyerror("Out of range for float conversion");
          return "";
        }

        if (value == "+")
          return to_string(convertedLeftValue + convertedRightValue);
        else if (value == "-")
          return to_string(convertedLeftValue - convertedRightValue);
        else if (value == "*")
          return to_string(convertedLeftValue * convertedRightValue);
        else if (value == "/")
          return to_string(convertedLeftValue / convertedRightValue);
      }
    }
    return "";
  }

  void print(int indent = 0)
  {
    for (int i = 0; i < indent; ++i)
      cout << "  ";

    cout << "Value: " << value << " | Type: " << type << endl;

    if (left != nullptr)
    {
      cout << "Left:\n";
      left->print(indent + 1);
    }

    if (right != nullptr)
    {
      cout << "Right:\n";
      right->print(indent + 1);
    }
  }
};