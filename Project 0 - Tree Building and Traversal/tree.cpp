//Chase Richards
//CMPSCI 4280 P0
//Filename: tree.cpp

#include "tree.h"

//Tree Constructor
Tree::Tree(){
   root = NULL;
}

//Build the tree from the words (names) 
Node* Tree::buildBinTree(istream& nameInput){
   string name;
      
   while(nameInput >> name){
      this->insert(name);
   }
   return root;
}

//Insert the words in the Binary tree
void Tree::insert(string value){
   root = insert(root, value);
}

Node* Tree::insert(Node* node, string name){
   //if it is a new node
   if(node == NULL)
      return new Node(name);
   
   //name is less than so traverse left
   if (name.at(0) < node->alphaCharacter)
      node->leftChild = insert(node->leftChild, name);
   
   //name is more than so traverse right
   else if (name.at(0) > node->alphaCharacter)
      node->rightChild = insert(node->rightChild, name);
   
   //name is equal to it so add to the set
   else if (name.at(0) == node->alphaCharacter)
      node->data.insert(name);
   
   return node;
}

//Preorder traversal file open and write the nodes in preorder
void Tree::preorderTraversal(Node* node, string file){
   //open the file as file.preorder
   file += ".preorder";
   ofstream outFile;
   outFile.open(file);

   //write the nodes into the file in preorder
   preorderTraversal(node, 0, outFile);
}

//Inorder traversal file open and write the nodes in inorder
void Tree::inorderTraversal(Node* node, string file){
   //open the file as file.inorder
   file += ".inorder";
   ofstream outFile;
   outFile.open(file);

   //write the nodes into the file in inorder
   inorderTraversal(node, 0, outFile);
}

//Postorder traversal file open and write the nodes in postorder
void Tree::postorderTraversal(Node* node, string file){
   //open the file as file.postorder
   file += ".postorder";
   ofstream outFile;
   outFile.open(file);

   //write the nodes into the file in postorder
   postorderTraversal(node, 0, outFile);
}

//traverse the tree in preorder taking the depths(levels) into account
void Tree::preorderTraversal(Node* node, int depth, ostream& nameOutput){
   if(node == NULL)
      return;
   //process root
   string tab((depth * 2), ' ');
   nameOutput << tab;
   for(auto name : node->data)
      nameOutput << name << " ";
   nameOutput << endl;
   //process left child
   preorderTraversal(node->leftChild, (depth + 1), nameOutput);
   //process right child
   preorderTraversal(node->rightChild, (depth + 1), nameOutput);
}

//traverse the tree inorder taking the depths (levels) into account 
void Tree::inorderTraversal(Node* node, int depth, ostream& nameOutput){
   if(node == NULL)
      return;
   //process left child
   inorderTraversal(node->leftChild, (depth + 1), nameOutput);
   //process root
   string tab((depth * 2), ' ');
   nameOutput << tab;
   for(auto name : node->data)
      nameOutput << name << " ";
   nameOutput << endl;
   //process right child
   inorderTraversal(node->rightChild, (depth + 1), nameOutput);
}

//traverse the tree in postorder taking the depths (levels) into account 
void Tree::postorderTraversal(Node* node, int depth, ostream& nameOutput){
   if(node == NULL)
      return;
   //process left child
   postorderTraversal(node->leftChild, (depth + 1), nameOutput);
   //process right child
   postorderTraversal(node->rightChild, (depth + 1), nameOutput);
   //process root
   string tab((depth * 2), ' ');
   nameOutput << tab;
   for(auto name : node->data)
      nameOutput << name << " ";
   nameOutput << endl;
}
