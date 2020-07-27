//Chase Richards
//CMPSCI 4280 P3
//Filename: parser.h

#ifndef PARSER_H
#define PARSER_H

#include "node.h"
#include "token.h"
using namespace std;

extern ifstream inFile;

Node* parser();

Node* program();
Node* block(int treeDepth);
Node* vars(int treeDepth);
Node* expr(int treeDepth);
Node* A(int treeDepth);
Node* N(int treeDepth);
Node* M(int treeDepth);
Node* R(int treeDepth);
Node* stats(int treeDepth);
Node* mstat(int treeDepth);
Node* stat(int treeDepth);
Node* in(int treeDepth);
Node* out(int treeDepth);
Node* iff(int treeDepth);
Node* loop(int treeDepth);
Node* assign(int treeDepth);
Node* RO(int treeDepth); 

#endif
