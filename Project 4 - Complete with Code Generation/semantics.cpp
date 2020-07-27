//Chase Richards
//CMPSCI 4280 P3
//Filename: semantics.cpp

#include <iostream>
#include <string>
#include "semantics.h"
using namespace std;

//max stack is 100
const int maxStack = 100;
//stack of tokens as an array
Token stack[maxStack];

int totVarCount = 0; //keep track of the total number of variables
int currScpFirstEle = 0; //keep track of the starting position of current scope

static int varTempCounter = 0;
static int labelCounter = 0;
string tempVarsCreated[maxStack];

void varTempArrays(){
   for(int i = 0; i <= maxStack; i++)
      tempVarsCreated[i] = "";
}
string getVarTemp(){
   string tempVarCreated = "V" + to_string(varTempCounter + 1);
   tempVarsCreated[varTempCounter] = tempVarCreated;
   varTempCounter++;
   return tempVarCreated;
}
string getLabel(){
   string labelCreated = "L" + to_string(labelCounter + 1);
   labelCounter++;
   return labelCreated;
}

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
      outFile << "PUSH\n";
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
      outFile << "POP\n";
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
      outFile << "STOP\n";
      for(int i = 0; i < maxStack; i++){
         if(tempVarsCreated[i] != "")
            outFile << tempVarsCreated[i] << " 0\n";
      }
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
         string tempVarCreated = getVarTemp();
         outFile << "STORE " << tempVarCreated << "\n";
         semantics(node -> secondChild, counter);
         outFile << "ADD " << tempVarCreated << "\n";
      }else{ //no plus token then we just have <A>
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<A>"){ //A nonTerminal
      //if there is a minus token then we have <N>-<A>
      if(node -> firstToken.identiToken == MinusTk){
         semantics(node -> firstChild, counter);
         string tempVarCreated = getVarTemp();
         outFile << "STORE " << tempVarCreated << "\n";
         semantics(node -> secondChild, counter);
         outFile << "SUB " << tempVarCreated << "\n";
      }else{ //otherwise we just have <N>
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<N>"){ //N nonTerminal
      //if we have a divide or mult token then we have either <M>/<N> or <M>*<N>
      if(node -> firstToken.identiToken == DivideTk){
         semantics(node -> secondChild, counter);
         string tempVarCreated = getVarTemp();
         outFile << "STORE " << tempVarCreated << "\n";
         semantics(node -> firstChild, counter);
         outFile << "DIV " << tempVarCreated << "\n";
      }else if(node -> firstToken.identiToken == MultiplicationTk){
         semantics(node -> secondChild, counter);
         string tempVarCreated = getVarTemp();
         outFile << "STORE " << tempVarCreated << "\n";
         semantics(node -> firstChild, counter);
         outFile << "MULT " << tempVarCreated << "\n";
      }else{ //otherwise we just have <M>
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<M>"){
      if(node -> firstToken.identiToken == MinusTk){
         semantics(node -> firstChild, counter);
         outFile << "MULT -1\n";
      }else{
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<R>"){ //R nonTerminal
      //if we have an identifier check to see if it has been declared in the current scope
      if(node -> firstToken.identiToken == IdTk){
         int locationOfVar = duplicateVarCheck(node -> firstToken);
         if(!duplicateVarCheck(node -> firstToken)){
            cout << "Error: " << node -> firstToken.stringToken << " not yet declared in current scope" << endl;
         }
         outFile << "STACKR " << locationOfVar << "\n";
      }else if(node -> firstToken.identiToken == IntTk){
         outFile << "LOAD " << node -> firstToken.stringToken << "\n";
      }else{
         //<expr> 
         semantics(node -> firstChild, counter);
      }
   }else if(node -> nonTerminal == "<in>"){ //in nonTerminal
      int locationOfVar = duplicateVarCheck(node -> firstToken); 
      if(!duplicateVarCheck(node -> firstToken)){ //check if the identifier has been declared in this scope
         cout << "Error:" << node -> firstToken.stringToken << " hasn't been declared in current scope" << endl;
         exit(EXIT_FAILURE);
      }
      string tempVarCreated = getVarTemp();
      outFile << "READ " << tempVarCreated << "\n";
      outFile << "LOAD " << tempVarCreated << "\n";
      outFile << "STACKW " << locationOfVar << "\n";
   }else if(node -> nonTerminal == "<out>"){
      semantics(node -> firstChild, counter);
      string tempVarCreated = getVarTemp();
      outFile << "STORE " << tempVarCreated << "\n";
      outFile << "WRITE " << tempVarCreated << "\n";
   }else if(node -> nonTerminal == "<if>"){
      idTokens firstIfOperator = node -> secondChild -> firstToken.identiToken;
      idTokens secondIfOperator = node -> secondChild -> secondToken.identiToken;
      semantics(node -> thirdChild, counter);
      string tempVarCreated = getVarTemp();
      outFile << "STORE " << tempVarCreated << "\n";
      semantics(node -> firstChild, counter);
      outFile << "SUB " << tempVarCreated << "\n";
      string labelCreated = getLabel();
      //figure out which RO operator it is
      if(firstIfOperator == LessThanTk){
         if(secondIfOperator == LessThanTk){
            outFile << "BRPOS " << labelCreated << "\n";
         }else if(secondIfOperator == GreaterThanTk){
/**********/outFile << "BRZERO " << labelCreated << "\n";
         }else{
            outFile << "BRZPOS " << labelCreated << "\n";
         }
      }else if(firstIfOperator == GreaterThanTk){
         if(secondIfOperator == GreaterThanTk){
            outFile << "BRNEG " << labelCreated << "\n";
         }else{
            outFile << "BRZNEG " << labelCreated << "\n";
         }
      }else if(firstIfOperator == EqualsTk){
         outFile << "BRPOS " << labelCreated << "\n";
         outFile << "BRNEG " << labelCreated << "\n";
      }
      if(node -> fourthChild != nullptr){
         semantics(node -> fourthChild, counter);
      }
      outFile << labelCreated << ": NOOP\n";
   }else if(node -> nonTerminal == "<loop>"){
      idTokens firstIfOperator = node -> secondChild -> firstToken.identiToken;
      idTokens secondIfOperator = node -> secondChild -> secondToken.identiToken;
      string tempVarCreated = getVarTemp();
      string startLabelCreated = getLabel();
      string stopLabelCreated = getLabel();
      outFile << startLabelCreated << ": NOOP\n";
      if(node -> thirdChild != nullptr){
         semantics(node -> thirdChild, counter);
      }
      outFile << "STORE " << tempVarCreated << "\n";
      semantics(node -> firstChild, counter);
      outFile << "SUB " << tempVarCreated << "\n";
      //figure out which ro operator is used
      if(firstIfOperator == LessThanTk){
         if(secondIfOperator == LessThanTk){
            outFile << "BRPOS " << stopLabelCreated << "\n";
         }else if(secondIfOperator == GreaterThanTk){
/**********/outFile << "BRZERO " << stopLabelCreated << "\n";
         }else{
            outFile << "BRZPOS " << stopLabelCreated << "\n";
         }
      }else if(firstIfOperator == GreaterThanTk){
         if(secondIfOperator == GreaterThanTk){
            outFile << "BRNEG " << stopLabelCreated << "\n";                                                   }else{                                                                                                   outFile << "BRZNEG " << stopLabelCreated << "\n";
         }
      }else if(firstIfOperator == EqualsTk){
         outFile << "BRPOS " << stopLabelCreated << "\n";
         outFile << "BRNEG " << stopLabelCreated << "\n";
      }
      if(node -> fourthChild != nullptr){
         semantics(node -> fourthChild, counter);
      }
      outFile << "BR " << startLabelCreated << "\n";
      outFile << stopLabelCreated << ": NOOP\n";
   }else if(node -> nonTerminal == "<assign>"){ //assign nonTerminal
      //<expr>
      semantics(node -> firstChild, counter);
      int locationOfVar = duplicateVarCheck(node -> firstToken);
      if(!duplicateVarCheck(node -> firstToken)){ //check if the identifier has been declared in this scope
         cout << "Error: " << node -> firstToken.stringToken << " hasn't been declared in current scope" << endl;
         exit(EXIT_FAILURE);
      }
      outFile << "STACKW " << locationOfVar << "\n";
   }else{
      semantics(node -> firstChild, counter);
      semantics(node -> secondChild, counter);
      semantics(node -> thirdChild, counter);
      semantics(node -> fourthChild, counter);
   }
}
