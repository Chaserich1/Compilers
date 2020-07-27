//Chase Richards
//CMPSCI 4280 P3
//Filename: node.h

#ifndef NODE_H
#define NODE_H

#include "token.h"
using namespace std;

struct Node{
   string nonTerminal;
   int treeDepth;
   //children for the <non-terminals>
   Node* firstChild;
   Node* secondChild;
   Node* thirdChild;
   Node* fourthChild;
   //Tokens to store the operaters, numbers, ids
   Token firstToken;
   Token secondToken;
   Token thirdToken;

   Node(){
      this -> nonTerminal = "";
      this -> treeDepth = 0;

      this -> firstChild = nullptr;
      this -> secondChild = nullptr;
      this -> thirdChild = nullptr;
      this -> fourthChild = nullptr;

      this -> firstToken = Token();
      this -> secondToken = Token();
      this -> thirdToken = Token();
   }   

   Node(string nonTerminal, int treeDepth){
      this -> nonTerminal = nonTerminal;
      this -> treeDepth = treeDepth;

      this -> firstChild = nullptr;
      this -> secondChild = nullptr;
      this -> thirdChild = nullptr;
      this -> fourthChild = nullptr;
   
      this -> firstToken = Token();
      this -> secondToken = Token();
      this -> thirdToken = Token();
   }
};

#endif    
