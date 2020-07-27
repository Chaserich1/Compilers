#ifndef TREE_H_
#define TREE_H_
#include "node.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
class Tree
{
   public:
      Node* root;
      //constructor and destructor
      Tree();
      //insert prototype
      void insert(string value);
      //build binary tree prototype
      Node* buildBinTree(istream& input);
      //all three prototypes for opening/creating file to write the tree in different orders
      void preorderTraversal(Node* node, string file);
      void inorderTraversal(Node* node, string file);
      void postorderTraversal(Node* node, string file);
   

   private:
      Node* insert(Node* node, string name);
      Node* clear(Node* node);
      
      //all three prototypes for traversing the words in different orders
      void preorderTraversal(Node* node, int depth, ostream& nameOutput);
      void inorderTraversal(Node* node, int depth, ostream& nameOutput);
      void postorderTraversal(Node* node, int depth, ostream& nameOutput);

};

#endif
