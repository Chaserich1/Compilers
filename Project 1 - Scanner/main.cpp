//Chase Richards
//CMPSCI 4280 PO
//Filename: main.cpp

#include <iostream>
#include <string>
#include <fstream>
#include "testScanner.h"
using namespace std;

int main (int argc, char** argv){
   
   //Make sure there are not more than 2 arguments
   if(argc > 2){
      cout << "Error: Too many arguments" << endl;
      return 0;
   }

   //if a filename was included in the arguments
   else if(argc == 2){
      string file = argv[1];
      //add extension .fs19 and open the file
      ifstream inFile;
      inFile.open(file + ".fs19");
      
      //if the file isn't opened
      if(!inFile){
         cout << "Program error file not found" << endl;
         return 0;   
      }

      testScanner(inFile);

      inFile.close();
   }

   //if there is no file argument, take keyboard input
   else if(argc == 1){
      
      //Create the file that will contain the keyboard input
      ofstream keyInputFile;
      //open the file
      keyInputFile.open("keyboard.txt");
      
      if(!keyInputFile){
         cout << "Error: couldn't open" << keyInputFile << endl;
         return 0;
      }

      //take in chars until the user ends the input
      char currCharacter;
      while (cin >> noskipws >> currCharacter)
         keyInputFile << currCharacter;
      
      //close the keyboard input fie
      keyInputFile.close();
    
      //keyboard input file
      ifstream inFile;
      inFile.open("keyboard.txt");

      //make sure inFile keyboard.txt is there
      if(!inFile){
         cout << "Program error keyboard.txt not found" << endl;
         return 0;
      }
      
      testScanner(inFile);
   
      inFile.close();
   }
}
         

   
