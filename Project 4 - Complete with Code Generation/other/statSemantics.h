#ifndef STATSEMANTICS_H
#define STATSEMANTICS_H

#include <iostream>
#include <fstream>
#include <vector>
#include "token.h"
#include "node.h"

using namespace std;

extern ofstream outFile;

struct stack_t 
{  
  Token tkn;
  int scope;
  int line_number;
};

//vector stack
extern vector<stack_t> stack;


//traverse tree and parse tree functions
void staticSemantics(node_t*);
void check_var(stack_t);
int check_var_exists(stack_t);
int find_var(stack_t);
int find_on_stack(stack_t);
int compare_scope(stack_t);
void remove_local_vars(int);
void print_stack();

#endif
