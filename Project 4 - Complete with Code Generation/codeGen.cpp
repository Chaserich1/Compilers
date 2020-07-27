#include <string>
#include <iostream>
#include <fstream>
#include "codeGen.h"

int varCounter = 0;
char newVar[9];

int labelCounter = 0;
char newLabel[9];

int variableCounter = 1;
char newVariable[9];

void codeGen(Node* node){
   if(node == NULL){
      return; 
   }
   programGen(node);
   outFile << "STOP \n";
   declareVars();
}

void programGen(Node* node){
   if(node == NULL){
      return; 
   }
   varsGens(node-> firstChild);
   blockGen(node-> secondChild);
   return;
}

void blockGen(Node* node){
   if(node == NULL){
      return; 
   }
   varsGens(node-> firstChild);
   statsGen(node-> secondChild);
   return;
}

void varsGens(Node* node){
   if(node == NULL){
      return; 
   }
   createVariable();
   varsGens(node-> firstChild);
   return;
}

void exprGen(Node* node){
   if(node == NULL){
      return; 
   }

   if(node-> secondChild != NULL){
      exprGen(node-> secondChild);
      char* tempVar = createTempVar();
      outFile << "STORE " << tempVar << "\n";
      AGen(node-> firstChild);
      if(node-> firstToken.identiToken == PlusTk){
         outFile << "ADD " << tempVar << "\n";
         return;
      }
   }else{
      AGen(node-> firstChild);
      return;
   }
}

void AGen(Node* node){
   if(node == NULL){
      return;
   }
   if(node-> secondChild != NULL){
      AGen(node-> secondChild);
      char* tempVar = createTempVar();
      outFile << "STORE " << tempVar << "\n";
      NGen(node-> firstChild);
      if(node-> firstToken.identiToken == MinusTk){
         outFile << "SUB " << tempVar << "\n";
         return;
      }
   }else{
      NGen(node-> firstChild);
      return;
   }
}

void NGen(Node* node){
   if(node == NULL){
      return; 
   }

   if(node-> secondChild != NULL){
       NGen(node-> secondChild);
       char* tempVar = createTempVar();
       outFile << "STORE " << tempVar << "\n";
       MGen(node-> firstChild);
       if(node-> firstToken.identiToken == DivideTk){
          outFile << "DIV " << tempVar << "\n";
          return;
       }else{
           outFile << "MULT " << tempVar << "\n";
           return;
       }
   }else{
       MGen(node-> firstChild);
       return;
   }
}

void MGen(Node* node){
   if (node == NULL){
       return; 
   }
   
   if(node-> firstToken.identiToken == MinusTk){
     MGen(node-> firstChild);
     outFile << "MULT -1\n";
     return;
   }else{
     RGen(node-> firstChild);
     return;
  }
}

void RGen(Node* node){
   if(node == NULL){
      return; 
   }

   if(node-> firstChild != NULL){
      exprGen(node-> firstChild);
      return;
   }else if(node-> firstToken.identiToken == IdTk){
      outFile << "LOAD " << node-> firstToken.stringToken << "\n";
      return;
   }else if(node-> firstToken.identiToken == IntTk){
      outFile << "LOAD " << node-> firstToken.stringToken << "\n";
      return;
   }
}

void statsGen(Node* node){
   if(node == NULL){
      return; 
   }
   
   statGen(node-> firstChild);
   mstatGen(node-> secondChild);
   return;
}

void mstatGen(Node* node){
   if(node == NULL){
      return; 
   }

   if(node-> firstChild != NULL){
      statGen(node-> firstChild);
      mstatGen(node-> secondChild);
      return;
   }
   return;
}

void statGen(Node* node){
   if(node == NULL){
      return; 
   }
   if(node-> firstChild-> nonTerminal == "<in>"){
      inGen(node-> firstChild);
      return;
   }else if(node-> firstChild-> nonTerminal == "<out>"){
      outGen(node-> firstChild);
      return;
   }else if(node-> firstChild-> nonTerminal == "<block>"){
      blockGen(node-> firstChild);
      return;
   }else if(node-> firstChild-> nonTerminal == "<if>"){
      ifGen(node-> firstChild);
      return;
   }else if(node-> firstChild-> nonTerminal == "<loop>"){
      loopGen(node-> firstChild);
      return;
   }else if(node-> firstChild-> nonTerminal == "<assign>"){
      AGenssign(node-> firstChild);
      return;
   }
}

void inGen(Node* node){
   if(node == NULL){
      return; 
   }
   outFile << "READ " << node-> firstToken.stringToken << "\n";
   return;
}

void outGen(Node* node){
   if(node == NULL){
       return; 
   }
   exprGen(node-> firstChild);
   char* tempVar = createTempVar();
   outFile << "STORE " << tempVar << "\n";
   outFile << "WRITE " << tempVar << "\n";
   return;
}


void ifGen(Node* node){
   if (node == NULL){
      return; 
   }
   exprGen(node-> thirdChild);
   char* tempVar = createTempVar();
   outFile << "STORE " << tempVar << "\n";
   exprGen(node-> firstChild);
   outFile << "SUB " << tempVar << "\n";
   char* label = createLabel();
   if((node-> secondChild-> firstToken.identiToken == LessThanTk) && (node-> secondChild-> secondToken.identiToken == LessThanTk)){
      outFile << "BRPOS " << label  << "\n";
   }
   else if((node-> secondChild-> firstToken.identiToken == GreaterThanTk) && (node-> secondChild-> secondToken.identiToken == GreaterThanTk)){
      outFile << "BRNEG " << label << "\n";
   }
   else if((node-> secondChild-> firstToken.identiToken == LessThanTk) && (node-> secondChild-> secondToken.identiToken == GreaterThanTk)){
      outFile << "BRZERO " << label << "\n";
   }
   else if(node-> secondChild-> firstToken.identiToken == LessThanTk){
      outFile << "BRZPOS " << label << "\n";
   }
   else if(node-> secondChild-> firstToken.identiToken == GreaterThanTk){
      outFile << "BRZNEG " << label << "\n";
   }
   else if(node-> secondChild-> firstToken.identiToken == EqualsTk){
      outFile << "BRNEG " << label << "\n";
      outFile << "BRPOS " << label << "\n";
   }
   statGen(node-> fourthChild);
   outFile << label << ": NOOP\n";
   return;
}

void loopGen(Node* node){
   if(node == NULL){
      return; 
   }
   char* label1 = createLabel();
   outFile << label1 << ": NOOP\n";
   exprGen(node-> thirdChild);
   char* tempVar = createTempVar();
   outFile << "STORE " << tempVar << "\n";
   exprGen(node-> firstChild);
   outFile << "SUB " << tempVar << "\n";
   char* label2 = createLabel();
   if((node-> secondChild-> firstToken.identiToken == LessThanTk) && (node-> secondChild-> secondToken.identiToken == LessThanTk)){
      outFile << "BRZPOS " << label2 << "\n";
   }
   else if((node-> secondChild-> firstToken.identiToken == GreaterThanTk) && (node-> secondChild-> secondToken.identiToken == GreaterThanTk)){
      outFile << "BRZNEG " << label2 << "\n";
   }
   else if((node-> secondChild-> firstToken.identiToken == LessThanTk) && (node-> secondChild-> secondToken.identiToken == GreaterThanTk)){
      outFile << "BRZERO " << label2 << "\n";
   }
   else if(node-> secondChild-> firstToken.identiToken == LessThanTk){
      outFile << "BRPOS " << label2 << "\n";
   }
   else if(node-> secondChild-> firstToken.identiToken == GreaterThanTk){
     outFile << "BRNEG " << label2 << "\n";
   }
   else if(node-> secondChild-> firstToken.identiToken == EqualsTk){
      outFile << "BRPOS " << label2 << "\n";
      outFile << "BRNEG " << label2 << "\n";
   }
   
   statGen(node-> fourthChild);
   outFile << "BR " << label1 << "\n";
   outFile << label2 << ": NOOP\n";
   return;
}

void AGenssign (Node* node){
   if(node == NULL){
      return; 
   }
   exprGen(node-> firstChild);
   outFile << "STORE " << node-> firstToken.stringToken << "\n";
   return;
}

char* createTempVar(){
   sprintf(newVar, "V%d", varCounter);
   varCounter++;
   return newVar;
}

char* createLabel(){
   sprintf(newLabel, "L%d", labelCounter);
   labelCounter++;
   return newLabel;
}

char* createVariable(){
   sprintf(newVariable, "x%d", variableCounter);
   variableCounter++;
   return newVariable;
}

void declareVars(){
   int i;
   for(i = 1; i < variableCounter; i++){
      sprintf(newVariable, "x%d", i);
      outFile << newVariable << " 0\n";
   }
   for(i = 0; i < varCounter; i++){
      sprintf(newVar, "V%d", i);
      outFile << newVar << " 0\n";
   }
}
                                           
