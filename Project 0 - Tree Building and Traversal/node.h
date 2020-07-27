//Chase Richards
//CMPSCI4280 P0
//Filename: node.h

#ifndef NODE_H_
#define NODE_H_
#include <string>
#include <set>

using namespace std;

struct Node{
   
   set <string> data;
   //the first letter of each word
   char alphaCharacter;
   //node pointer to the direction (to the left or right child)
   Node* rightChild;
   Node* leftChild;

   Node(string data){
      this->data.insert(data);
      this->alphaCharacter = data.at(0);
      this->rightChild = nullptr;
      this->leftChild = nullptr;
   }

};

#endif
