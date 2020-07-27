//Chase Richards
//CMPSCI 4280 P3
//Filename: testTree.cpp

#include "parser.h"
#include "scanner.h"
#include <iostream>

void treePrinted(Node* node)
{
    if(node == nullptr)
    {
        cout << "The tree is empty." << endl;
        return;
    }
    else
    {
        string indents((node->treeDepth * 2), ' ');
        cout << indents;
        cout << node->nonTerminal << " ";
        cout << node->firstToken.stringToken << "  " << node->secondToken.stringToken << "  " << node->thirdToken.stringToken;
        cout << endl;

        if(node->firstChild != nullptr)
            treePrinted(node->firstChild);
        if(node->secondChild != nullptr)
            treePrinted(node->secondChild);
        if(node->thirdChild != nullptr)
            treePrinted(node->thirdChild);
        if(node->fourthChild != nullptr)
            treePrinted(node->fourthChild);
    }
    return;
}
