//Chase Richards
//CMSPCI 4280 P3
//Filename: semantics.h

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <fstream>
#include "node.h"
#include "token.h"

extern ofstream outFile;

//push and pop
void push(Token tkn);
void pop(int currScpFirstEle);
int find(Token tkn);
//check if var has already been defined
bool duplicateVarCheck(Token tkn);
//build the stack and static semantics
void stacker();
void semantics(Node* node, int counter);

#endif
