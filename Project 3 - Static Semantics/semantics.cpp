//Chase Richards
//CMPSCI 4280 P3
//Filename: semantics.cpp

#include "semantics.h"
#include <iostream>

//max stack is 100
const int maxStack = 100;
//stack of tokens as an array
Token stack[maxStack];


int totVarCount = 0; //keep track of the total number of variables
int currScpFirstEle = 0; //keep track of the starting position of current scope

//Push onto the stack
void push(Token tkn){
   //If we have more than 100 items then overflow because stack size is 100
   if(totVarCount > maxStack){
      cout << "Error: Stack Overflow" << endl;
      exit(EXIT_FAILURE);
   }else{
      for(int i = currScpFirstEle; i < totVarCount; i++){
         //Check if the variable has already been defined in the current scope and if it has exit
         if(stack[i].stringToken == tkn.stringToken){
            cout << "Error: " << tkn.stringToken << " already declared in current scope" << endl;
            exit(EXIT_FAILURE);
         }
      }
      stack[totVarCount] = tkn; //set tkn to the total variable count in the stack
      totVarCount++; //increment the total variable count
   }
}

/*Take in the first element of the current scope and pop from the 
 stack all of the elements on the stack from the end back to 
 the first element of the current scope*/
void pop(int currScpFirstEle){
   //Start at the top of the stack and go through the current scope
   for(int i = totVarCount; i > currScpFirstEle; i--){
      totVarCount--; //decrement total variable count
      stack[i].stringToken == ""; 
   }
}

/*find the item on the stack and return the distance from the
  top if found, otherwise return -1 if it is not found*/
int find(Token tkn){
   /*start on the top of the stack and go down until the 
     first element of the current scope*/
   for(int i = totVarCount; i >= currScpFirstEle; i--){ 
      /*If we find the item then return the distance from 
        the top of the stack*/
      if(stack[i].stringToken == tkn.stringToken){
         int TOS = totVarCount - 1 - i;
         return TOS;
      }
   }
   return -1; //return -1 if the item is not found
}

//Check if variable already exists. Return true if it does, false if not
bool duplicateVarCheck(Token tkn){
   for(int i = totVarCount - 1; i > -1; i--){
      if(stack[i].stringToken == tkn.stringToken){
         return true;
      }
   }
   return false;
}

//Static semantics function, different actions based on the subtree and node
void semantics(Node* node, int counter){
   //empty tree
   if(node == nullptr){
      return;
   }
   //left to right traversal with different nonterminals, nodes and actions
   if(node -> nonTerminal == "<program>"){ //proram nonTerminal
      int varCount = 0; //initialize variable counter to 0
      currScpFirstEle = totVarCount;
      semantics(node -> firstChild, varCount);
      semantics(node -> secondChild, varCount);
   }else if(node -> nonTerminal == "<block>"){ //block nonTerminal
         int varCount = 0; //set varCount to 0 for this block
         //set first item in the scope equal to total variable count
         currScpFirstEle = totVarCount; 
         semantics(node -> firstChild, varCount);
         semantics(node -> secondChild, varCount);
         //pop the current scope       
         pop(currScpFirstEle);
   }else if(node -> nonTerminal == "<vars>"){ //vars nonTerminal
      //find the first token and set it to top of stack
      int TOS;
      TOS = find(node -> firstToken);
      //set first element of the scope to the total variable count
      currScpFirstEle = totVarCount;
      //if top of stack is -1 or greater than the count then push the token onto the stack and increment counter
      if(TOS == -1 || TOS > counter){
         push(node -> firstToken);
         counter++;
      }else if(TOS < counter){ //if top of stack is less than counter then the the tken has already been declared in scope
         cout << "Error: " << node -> firstToken.stringToken << " already declared in current scope" << endl;
         exit(EXIT_FAILURE);
      }
      //empty or <vars>
      if(node -> firstChild != nullptr){
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<expr>"){ //expr nonTerminal
      //if there is a plus token then we have <A>+<expr>
      if(node -> firstToken.identiToken == PlusTk){
         semantics(node -> firstChild, counter);
         semantics(node -> secondChild, counter);
      }else{ //no plus token then we just have <A>
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<A>"){ //A nonTerminal
      //if there is a minus token then we have <N>-<A>
      if(node -> firstToken.identiToken == MinusTk){
         semantics(node -> firstChild, counter);
         semantics(node -> secondChild, counter);
      }else{ //otherwise we just have <N>
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<N>"){ //N nonTerminal
      //if we have a divide or mult token then we have either <M>/<N> or <M>*<N>
      if(node -> firstToken.identiToken == DivideTk || node -> firstToken.identiToken == MultiplicationTk){
         semantics(node -> firstChild, counter);
         semantics(node -> secondChild, counter);
      }else{ //otherwise we just have <M>
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<R>"){ //R nonTerminal
      //if we have an identifier check to see if it has been declared in the current scope
      if(node -> firstToken.identiToken == IdTk){
         if(!duplicateVarCheck(node -> firstToken)){
            cout << "Error: " << node -> firstToken.stringToken << " not yet declared in current scope" << endl;
         }
      }else{
         //<expr> 
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<in>"){ //in nonTerminal
      if(!duplicateVarCheck(node -> firstToken)){ //check if the identifier has been declared in this scope
         cout << "Error:" << node -> firstToken.stringToken << " hasn't been declared in current scope" << endl;
         exit(EXIT_FAILURE);
      }
   }else if(node -> nonTerminal == "<assign>"){ //assign nonTerminal
      if(!duplicateVarCheck(node -> firstToken)){ //check if the identifier has been declared in this scope
         cout << "Error: " << node -> firstToken.stringToken << " hasn't been declared in current scope" << endl;
         exit(EXIT_FAILURE);
      }
      //<expr>
      semantics(node -> firstChild, counter);
   }else{
      semantics(node -> firstChild, counter);
      semantics(node -> secondChild, counter);
      semantics(node -> thirdChild, counter);
      semantics(node -> fourthChild, counter);
   }
}
