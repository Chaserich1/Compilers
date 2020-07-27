#include <set>
#include <string>
#include <stdio.h>
#include "statSemantics.h"
#include "node.h"
using namespace std;

set <string> variables;

int verify(string var){
   if(variables.count(var) == 0){
      return -1;
   }
   return 1;
}

int insert(string var){
   if(variables.count(var) == 0){
      variables.insert(var);
      return 1;
   }
   return -1;
}

void semantics(Node* node){
   if(node == NULL){
      return;
   }
   
   if(node -> nonTerminal == "<vars>"){
      if(insert(node -> firstToken.stringToken) < 0){
         printf("Variable on line %d already initialized\n", node -> firstToken.lineNum);
         exit(EXIT_FAILURE);
      }
   }/*else if(node -> nonTerminal == "<R>"){
      if(verify(node -> firstToken.stringToken) < 0){
         printf("Variable on line %d not declared\n", node -> firstToken.lineNum);
         exit(EXIT_FAILURE);
      }
   }else if(node -> nonTerminal == "<in>"){
      if(verify(node -> firstToken.stringToken) < 0){
         printf("Variable on line %d not declared\n", node -> firstToken.lineNum);
         exit(EXIT_FAILURE);
      }
   }else if(node -> nonTerminal == "<assign>"){
      if(verify(node -> firstToken.stringToken) < 0){
         printf("Variable on line %d not declared\n", node -> firstToken.lineNum);
         exit(EXIT_FAILURE);
      }
   }*/
   if(node-> firstChild != NULL){ 
      semantics(node -> firstChild);
   }
   if(node-> secondChild != NULL){
      semantics(node -> secondChild);
   }
   if(node-> thirdChild != NULL){
      semantics(node -> thirdChild);
   }
   if(node-> fourthChild != NULL){
      semantics(node -> fourthChild);
   }
}
