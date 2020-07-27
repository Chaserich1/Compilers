//Chase Richards
//CMPSCI 4280 PO
//Filename: main.cpp

#include <iostream>
#include <string>
#include <fstream>
#include "tree.h"
using namespace std;

int main (int argc, char** argv){
   
   //Make sure there are not more than 2 arguments
   if(argc > 2){
      printf("Error: Too many arguments\n");
      return 0;
   }

   //if a filename was included in the arguments
   if(argc == 2){
      string file = argv[1];
      //add extension .fs19 and open the file
      ifstream inFile;
      inFile.open(file + ".fs19");
      
      //if the file exists else error
      if(inFile){
         //check if the file is empty
         if(inFile.peek() == ifstream::traits_type::eof()){
            cout << "Error: Input file is empty. Please use a file with data in it. Terminating...\n";
            return 0;
         }

         //create the binary tree
         Tree binTree;
         //use the input file and build the tree
         binTree.root = binTree.buildBinTree(inFile);

         //Preorder tree traversal and print to preorder file
         binTree.preorderTraversal(binTree.root, file);
         //Inorder tree traversal and print to inorder file
         binTree.inorderTraversal(binTree.root, file);
         //Postorder tree traversal and print to postorder file
         binTree.postorderTraversal(binTree.root, file);

         inFile.close();

         return 0;
      }
      //else the file isn't found
      else{
         cout << "Error: Unable to find the file" << endl;
         return 0;
      }
   }

   //if there is no file argument, take keyboard input
   if(argc == 1){
      
      //Create the file that will contain the keyboard input
      ofstream keyInputFile;
      //open the file
      keyInputFile.open("keyboard.txt");
      
      //take in names until the user types EOF to end the input
      string name = "";
      while (cin >> name)
         keyInputFile << (name + ' ');
      
      //close the keyboard input fie
      keyInputFile.close();
    
      //keyboard input file
      ifstream inFile;
      inFile.open("keyboard.txt", ios::in);

      //make sure inFile keyboard.txt is there
      if(inFile){
         //check if the file is empty
         if(inFile.peek() == ifstream::traits_type::eof()){
            cout << "Error: Keyboard input file is empty. Please use a file with data in it. Terminating...\n";
            return 0;
         }
         //create the binary tree
         Tree binTree;
         //take the words from the keyboard.txt and fill the tree
         binTree.root = binTree.buildBinTree(inFile);
         //Preorder tree traversal and print to preorder file
         binTree.preorderTraversal(binTree.root, "tree");
         //Inorder tree traversal and print to inorder file
         binTree.inorderTraversal(binTree.root, "tree");
         //Postorder tree traversal and print to postorder file
         binTree.postorderTraversal(binTree.root, "tree");

         inFile.close();
         return 0;
      }
      //if there is no inFile keyboard.txt
      else{
         cout << "Error: Unable to find the file" << endl;
         return 0;
      }
   }
   return 0;
}
         

   
