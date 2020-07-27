//Chase Richards
//CMPSCI 4280 P3
//Filename: scanner.cpp

#include "scanner.h"
#include "token.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int fsaTable[24][26] = {                                                                                                    
   // ws  lower    d    =    <    >   <=   >=   ==    :    +    -    *    /    %    .    (    )    ,    {    }    ;    [    ]  eof  upper                                                                                                                         
   {     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25},
   {   100,   1,   1, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,   1}, //IdTk
   {   101, 101,   2, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101}, //IntTk
   {   102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102}, //EqualsTk
   {   103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103}, //LessThanTk
   {   104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104}, //GreaterThanTk
   {   105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105}, //LessThanEqualTk
   {   106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106}, //GreaterThanEqualTk               
   {   107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107}, //EqualEqualTk                     
   {   108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108}, //ColonTk                          
   {   109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109}, //PlusTk                        
   {   110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110}, //MinusTk
   {   111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111}, //MultiplicationTk
   {   112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112}, //DivideTk
   {   113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113}, //PercentTk
   {   114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114}, //PeriodTk
   {   115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115}, //LeftParenthesesTk
   {   116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116}, //RightParenthesesTk
   {   117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117}, //CommaTk
   {   118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118}, //LeftBraceTk
   {   119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119}, //RightBraceTk
   {   120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120}, //SemiColonTk
   {   121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121}, //LeftSquareBrackTk
   {   122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122}, //RightSquareBrackTk
};    

//Map the input string keywords to their keyword tokens
map<string, idTokens> keywordTokens = {
   {"start", StartTk},{"stop", StopTk},{"iterate", IterateTk},{"void", VoidTk},{"var", VarTk},{"return", ReturnTk},
   {"in", InTk},{"out", OutTk},{"program", ProgramTk},{"cond", CondTk},{"then", ThenTk},{"let", LetTk}
};

//Map the input symbols to their symbol tokens 
map<char, int> symbolTokens = {
   {'=', 3},{'<', 4},{'>', 5},/*{'<=', 6},{'>=', 7},{'==', 8},*/{':', 9},{'+', 10},{'-', 11},{'*', 12},{'/', 13},
   {'%', 14},{'.', 15},{'(', 16},{')', 17},{',', 18},{'{', 19},{'}', 20},{';', 21},{'[', 22},{']', 23}
};

//Map the final fsatable states to the tokens
map<int, idTokens> finalStates = {
   //Identifier and Integer Tokens
   {100, IdTk},{101, IntTk},
   //Symbol Tokens
   {102, EqualsTk},{103, LessThanTk},{104, GreaterThanTk},{105, LessThanEqualTk},{106, GreaterThanEqualTk},
   {107, EqualEqualTk},{108, ColonTk},{109, PlusTk},{110, MinusTk},{111, MultiplicationTk},{112, DivideTk},
   {113, PercentTk},{114, PeriodTk},{115, LeftParenthesesTk},{116, RightParenthesesTk},{117, CommaTk},
   {118, LeftBraceTk},{119, RightBraceTk},{120, SemiColonTk},{121, LeftSquareBrackTk},{122, RightSquareBrackTk},
   //End of file token
   {24, EofTk}
};

//Function: scanner
Token scanner(ifstream &inFile, int &lineNum){
   //Initialize the current and next state beginning at the 0 state
   int currState = 0;
   int nextState = 0;
   char currCharacter = ' '; //currCharacter to hold the current character 
   string currWord = ""; //currWord holds string of chars for one idToken

   //while not a final state and not end of file get the characters
   while (currState < 100 && currState >= 0 && currState < 24){
      //Get the next character
      inFile.get(currCharacter);
      //If character is #, then get characters as comment until newline 
      if (currCharacter == '#'){
         do{
            inFile.get(currCharacter);
         }while (currCharacter != '\n');
         //currCharacter is set to # so get the next character and continue
         lineNum++;
         inFile.get(currCharacter);
      }      
      //set fsaColumn variable to the column based on the current character
      int fsaColumn = getColumn(currCharacter); 
      //fasColumn 24 is when the end of the file is reached
      if(inFile.eof()){
         fsaColumn = 24;
      }
      //If a character is entered that is not a valid token, then return the error token
      if(fsaColumn == 25){
         cout << "SCANNER ERROR: Invalid character " << currCharacter << " on line " << lineNum << endl;
         return Token(ErrorTk, "Invalid character", lineNum);
      }
      //set the next state
      nextState = fsaTable[currState][fsaColumn];
      //check if the next state is a final state
      if (nextState == 24 || nextState == 25 || nextState >= 100){
         //If the next state is eof then return the eoftk
         if(nextState == 24){
            return Token(EofTk, "EOF", 0);
         }
         if(nextState == 25){
            //If a character is entered that is not valid, then show the details and return the error token
            cout << "SCANNER ERROR: Invalid character" << currCharacter << " on line " << lineNum << endl;
            return Token(ErrorTk, "Invalid ID", lineNum);
         }
         //states for keywords and symbols return the token
         inFile.unget();
         return getToken(nextState, currWord, lineNum);
      }else{
         //if current character is <, get the next character if there is no space and the next character is =
         if(!isspace(currCharacter) && currCharacter == '<'){
            inFile.get(currCharacter);
            if(currCharacter == '='){
               return Token(LessThanEqualTk, "<=", lineNum);
            }
            lineNum++;
            return Token(LessThanTk, "<", lineNum - 1);
         }
         //if current character is >, get the next character if there is no space and the next character is =
         if(!isspace(currCharacter) && currCharacter == '>'){
            inFile.get(currCharacter);
            if(currCharacter == '='){
               return Token(GreaterThanEqualTk, ">=", lineNum);
            }
            lineNum++;
            return Token(GreaterThanTk, ">", lineNum - 1);
         }
         //if current character is =, get the next character if there is no space and the next character is =
         if(!isspace(currCharacter) && currCharacter == '='){
            inFile.get(currCharacter);
            if(currCharacter == '='){
               return Token(EqualEqualTk, "==", lineNum);
            }
            lineNum++;
            return Token(EqualsTk, "=", lineNum - 1);
         } 
         //If there is no space/new line, then add the input characters to the current word
         if(!isspace(currCharacter)){
            currWord += currCharacter;
         } 
         //if the word/number is larger than 8 characters then scanner error and return the error token  
         if (currWord.length() > 8){
            cout << "SCANNER ERROR: " << currWord << "is more than 8 characters" << " on line " << lineNum << endl; 
            return Token(ErrorTk, "Invalid Length", lineNum);
         }
         //increment the line number
         if(currCharacter == '\n'){
            lineNum++;
         }
         //go from the current state to the next state
         currState = nextState;
      }
   }
   //return error token here if there has not alreadt been a token returned
   return Token(ErrorTk, "SCANNER ERROR", lineNum);
}

//Getter for the tokens
Token getToken(int state, string word, int lineNum){
   //Default returned token
   Token tokenReturned = Token();
   //setter for the tokens
   if (finalStates.find(state) != finalStates.end()){
      tokenReturned.identiToken = finalStates[state];
      tokenReturned.stringToken = word;
      tokenReturned.lineNum = lineNum;
   }
   //if it is one of the keyword tokens then change the token to the keyword token
   if (keywordTokens.find(word) != keywordTokens.end()){
      tokenReturned.identiToken = keywordTokens[word];
   }
   //return a default token if the token returned is not changed by the above
   return tokenReturned;
}

//Getter for the column in the fsa table
int getColumn(char currCharacter){
    //Input value is a letter, return column 1 from the fsa table
   if (isalpha(currCharacter)){
      if(isupper(currCharacter))
         return 25;
      return 1;
   }
   //Input value is an integer, return column 2 from the fsa table
   else if (isdigit(currCharacter))
      return 2;
   //Input value is a whitespace, return column 0 from the fsa table
   else if (isspace(currCharacter))
      return 0;   
   //If it has reached the end of the file, return column 24 
   else if (currCharacter == EOF)
      return 24;
   //Check if the symbol is in the symbol map
   else{
      //return the column if it is in the symbol map,
      if (symbolTokens.find(currCharacter) != symbolTokens.end()){
         return symbolTokens[currCharacter];
      }
   }
   //return error token because the symbol isn't allowed
   return 25;
}
