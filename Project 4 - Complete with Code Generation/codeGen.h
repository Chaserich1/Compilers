#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "node.h"

extern ofstream outFile;

void codeGen(Node* node);
void programGen(Node* node);
void blockGen(Node* node);
void varsGens(Node* node);
void exprGen(Node* node);
void AGen(Node* node);
void NGen(Node* node);
void MGen(Node* node);
void RGen(Node* node);
void statsGen(Node* node);
void mstatGen(Node* node);
void statGen(Node* node);
void inGen(Node* node);
void outGen(Node* node);
void ifGen(Node* node);
void loopGen(Node* node);
void AGenssign(Node* node);

char* createVariable();
char* createTempVar();
char* createLabel();

void declareVars();

#endif
