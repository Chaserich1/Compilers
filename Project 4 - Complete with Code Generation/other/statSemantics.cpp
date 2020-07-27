#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "statSemantics.h"
#include "token.h"
#include "node.h"

using namespace std;

/*max stack size*/
const int MAX_VARS = 100;

/*Var scope*/
static int var_scope = 0;

/*temp vars*/
static int temp_vars = 0;

static int num_repeat = 0;

/*stack temp*/
static stack_t temp;

/*vector stack for vars*/
vector<stack_t> stack;

/*location on stack*/
int stack_location = 0;

/*Total vars on stack*/
int stack_vars = 0;


void staticSemantics(Node *node)
{
  if (node == NULL)
    return;

  /*<program> -> void <vars> <block>*/
  if (node->nonTerminal == "<program>")
  {
    staticSemantics(node->firstChild); 
    staticSemantics(node->secondChild); 

    remove_local_vars(var_scope);
    var_scope--;

    /*out.asm STOP*/
    outFile << "STOP\n";

    while (temp_vars > 0)
    {
      outFile << "T" << (temp_vars - 1) << " 0\n";
      temp_vars--;
    }

    return;
  }

  /*<block> -> begin <vars> <stats> end*/
  if (node->nonTerminal == "<block>")
  {
    var_scope++;

    staticSemantics(node->firstChild); 
    staticSemantics(node->secondChild); 

    remove_local_vars(var_scope);
    var_scope--;
  }

  /*<vars> -> empty | let Identifier = Integer <vars>*/
  if (node->nonTerminal == "<vars>")
  {
    temp.token = node->tkn.front();
    temp.scope = var_scope;

    if (temp.token.desc != "EMPTY")
    {
      check_var(temp);

      stack_vars++;

      staticSemantics(node->firstChild);
    }
  }

  /*<expr> -> <A> / <expr> | <A> * <expr> | <A>*/
  if (node->nonTerminal == "<expr>")
  {
    if (node->tkn.empty())
      staticSemantics(node->firstChild); 
    else
    {
      int local_var = temp_vars;
      temp_vars++;
      staticSemantics(node->secondChild);
     
      outFile << "STORE T" << local_var << "\n";
      staticSemantics(node->firstChild);

      if (node -> tkn.identiToken == PlusTk)
      {

        outFile << "ADD T" << local_var << "\n";
      }
      return;
    }
  }
  if(node->nonTerminal == "<A>"){
    if (node->tkn.empty())
      staticSemantics(node->firstChild);
    else
    {
      int local_var = temp_vars;
      temp_vars++;
      staticSemantics(node->secondChild);

      outFile << "STORE T" << local_var << "\n";
      staticSemantics(node->firstChild);

      if (node -> tkn.identiToken == MinusTk){
        outFile << "SUB T" << local_var << "\n";                                                      }
      return;
    }
  /*<A> -> <M> + <A> | <M> - <A> | <M>*/
  if (node->nonTerminal == "<N>")
  {
    if (node->tkn.empty())
      staticSemantics(node->firstChild);
    else
    {
      int local_var = temp_vars;
      temp_vars++;
     
      staticSemantics(node->secondChild);
      outFile << "STORE T" << local_var << "\n";
      staticSemantics(node->firstChild);

      if (node -> tkn.identiToken == DivideTk)
      {
        outFile << "DIV T" << local_var << "\n";
      }
      else if (node -> tkn.identiToken == MultiplicationTk)
      {
        outFile << "MULT T" << local_var << "\n";
      }
      return;
    }
  }

  /*<M> -> -<M> | <R>*/
  if (node->nonTerminal == "<M>")
  {
    staticSemantics(node->firstChild);

    if (!node->tkn.empty())
    {
      outFile << "MULT -1\n";
    }
    return;
  }

  /*<R> -> (<expr>) | Identifier | Integer*/
  if (node->nonTerminal == "<R>")
  {
    if (node->tkn.empty())
      staticSemantics(node->firstChild); 
    else
    {
      temp.token = node->tkn.front();
      temp.scope = var_scope;

      if (node -> firstToken.identiToken == IdTk)
      {
        compare_scope(temp);
        outFile << "STACKR " << find_on_stack(temp) << "\n";
      }
      else if (node -> firstToken.identiToken == IntTk)
      {
        outFile << "LOAD " << get_token_int(temp.token) << "\n";
      }
      return;
    }
  }

  /*<stats> -> <stat> <mStat>*/
  if (node->nonTerminal == "<stats>")
  {
    staticSemantics(node->firstChild); 
    staticSemantics(node->secondChild); 
  }

  /*<stat> -> <in> | <out> | <block> | <if> | <loop> | <assign>*/
  if (node->nonTerminal == "<stat>")
  {
    staticSemantics(node->firstChild); 
  }
  
  /*<mStat> -> empty | <stat> <mStat>*/
  if (node->nonTerminal == "<mStat>")
  {
    if (node->tkn.empty())
    {
      staticSemantics(node->firstChild); 
      staticSemantics(node->secondChild); 
    }
  }

  /*<in> -> read ( Identifier ):*/
  if (node->nonTerminal == "<in>")
  {
    temp.token = node->tkn.front();
    temp.scope = var_scope;

    int local_var = temp_vars;
    temp_vars++;
    compare_scope(temp);

    outFile << "READ T" << local_var << "\n";
    outFile << "LOAD T" << local_var << "\n";
    outFile << "STACKW " << find_on_stack(temp) << "\n";

    return;
  }

  /*<out> -> print ( <expr> ) :*/
  if (node->nonTerminal == "<out>")
  {
    int local_var = temp_vars;
    temp_vars++;

    staticSemantics(node->firstChild);

    outFile << "STORE T" << local_var << "\n";
    outFile << "WRITE T" << local_var << "\n";

    return;
  }

  /*<if> -> cond (<expr> <RO> <expr>) <stat>*/
  if (node->nonTerminal == "<if>")
  {
    int loop = ++num_repeat;
    int local_var = temp_vars;
    temp_vars++;

    outFile << "loop" << loop << ": ";
    staticSemantics(node->firstChild);

    outFile << "STORE T" << local_var << "\n";
    staticSemantics(node->thirdChild); 
    outFile << "SUB T" << local_var << "\n";

    staticSemantics(node->secondChild); 
    staticSemantics(node->fourthChild); 
    outFile << "branch" << loop << ": NOOP\n";

    return;
  }

  /*<loop> -> iter (<expr> <RO> <expr>) <stat>*/
  if (node->nonTerminal == "<loop>")
  {
    int loop = ++num_repeat;
    int local_var = temp_vars;
    temp_vars++;

    outFile << "loop" << local_var << ": ";

    staticSemantics(node->firstChild); 
    outFile << "STORE T" << local_var << "\n";

    staticSemantics(node->thirdChild);
    outFile << "SUB T" << local_var << "\n";

    staticSemantics(node->secondChild); 
    staticSemantics(node->fourthChild); 
    outFile << "BR loop" << loop << "\n";
    outFile << "branch" << loop << ": NOOP\n";
  }

  /*<assign> -> Identifier = <expr> :*/
  if (node->nonTerminal == "<assign>")
  {
    temp.token = node->tkn.front();
    temp.scope = var_scope;
    compare_scope(temp);

    staticSemantics(node->firstChild);

    temp.token = node->tkn.front();
    temp.scope = var_scope;
    outFile << "STACKW " << find_on_stack(temp) << "\n";

    return;
  }
/*
  if (node->nonTerminal == "<RO>")
  {
    temp.token = node->tkn.front();
    temp.scope = var_scope;
    node->tkn.erase(node->tkn.begin());
    if (operator_map[temp.token.desc] == "LESS_tk")
    {
      if (!node->tkn.empty())
      {
        stack_t temp_2;
        temp_2.token = node->tkn.front();
        temp_2.scope = var_scope;
        node->tkn.erase(node->tkn.begin());

        if (node -> firstToken.identiToken == LessThanTk)
        {
          outFile << "BRNEG branch" << num_repeat << "\n";
          return;
        }
      }

      outFile << "BRZNEG branch" << num_repeat << "\n";
      return;
    }
    else if (operator_map[temp.token.desc] == "GREAT_tk")
    {
      if (!node->tkn.empty())
      {
        stack_t temp_2;
        temp_2.token = node->tkn.front();
        temp_2.scope = var_scope;
        node->tkn.erase(node->tkn.begin());

        cout << "RO production: token is " << temp_2.token.desc << "\n";

        if (operator_map[temp_2.token.desc] == "EQUALS_tk")
        {
          outFile << "BRPOS branch" << num_repeat << "\n";
          return;
        }
      }
      outFile << "BRZPOS branch" << num_repeat << "\n";
      return;
    }
    else if (operator_map[temp.token.desc] == "EQUALS_tk")
    {
      if (!node->tkn.empty())
      {
        stack_t temp_2;
        temp_2.token = node->tkn.front();
        temp_2.scope = var_scope;
        node->tkn.erase(node->tkn.begin());

        if (operator_map[temp_2.token.desc] == "EQUALS_tk")
        {
          outFile << "BRZERO branch" << num_repeat << "\n";
          return;
        }
      }

      outFile << "BRPOS branch" << num_repeat << "\n";
      outFile << "BRNEG branch" << num_repeat << "\n";
      return;
    }
  }
}
*/
/*-----------------------------------------------*/
/*check top of stack var*/
void check_var(stack_t var)
{
  int var_defined = find_var(var);
  if (var_defined > 0)
  {
    cout << "Error: staticSemantics variable \'" << 
      get_token_desc(var.token) << "\' on line " << var.token.line_number 
      << " has already been defined in this scope on line " 
      << stack.at(var_defined).token.line_number << ".\n";
    exit(EXIT_FAILURE);
  }
  else
  {
    stack.push_back(var);
    stack_location++;
    outFile << "PUSH\n";
  }
}

/*------------------------------------------------------*/
/*check if var exists on stack*/
int check_var_exists(stack_t var)
{
  for (int counter = 0; counter < stack.size(); counter++)
  {
    if (stack.at(counter).token.desc == var.token.desc)
      return counter;
  }
  return -1;
}

/*-------------------------------------------------------*/
/*find var on stack*/
int find_var(stack_t var)
{
  for (int counter = 0; counter < stack.size(); counter++)
  {
    if ((stack.at(counter).token.desc == var.token.desc) &&
      (stack.at(counter).scope == var.scope))
    {
      return counter;
    }
  }

  return -1;
}
/*-------------------------------------------------------*/
/*find the location on stack*/
int find_on_stack(stack_t var)
{
  for (int counter = 0; counter < stack.size(); counter++)
  {
    if ((stack.at(counter).token.desc == var.token.desc) &&
      (stack.at(counter).scope <= var.scope))
      return counter;
  }
  cout << "The identifier " << var.token.desc << " was not declared in "
    << "this scope.\n";
  exit(EXIT_FAILURE);
}

/*check local and global scopes*/
int compare_scope(stack_t var)
{
  int var_location = check_var_exists(var);

  if (var_location >= 0)
  {
    if (stack.at(var_location).scope > var.scope)
    {
      cout << "Error: staticSemantics  variable \'" << get_token_desc(var.token) 
        << "\' on line " << var.token.line_number << 
        " cannot be accessed in this scope.\n";
      exit(EXIT_FAILURE);
    }
    else
      return 1;
  }
  else
  {
   
    cout << "Error: staticSemantics variable \'" << get_token_desc(var.token)
      << "\' on line " << var.token.line_number << 
      " is not on the stack, and has either not yet been defined OR "
      << "cannot be accessed in this scope.\n";
    exit(EXIT_FAILURE);
  }
}

/*delete all vars leaving block*/
void remove_local_vars(int scope)
{
  if (scope > 0)
  {
    for (int counter = 0; counter < stack.size(); counter++)
    {
      if (stack.at(counter).scope == scope)
      {
        outFile << "POP\n";
        stack.erase(stack.begin() + counter);
      }
    }
  }
}

/*---------------------------------------------------------*/
/*function to print stack*/
void print_stack()
{
  for (int i = 0; i < stack.size(); i++)
  {
    cout << "\tIndex " << i << " = " << stack.at(i).token.desc << 
      ", scope is " << stack.at(i).scope << "\n";
  }
}
