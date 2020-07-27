//Chase Richards
//CMPSCI 4280 P1
//Filename: testScanner.cpp

#include "scanner.h"
#include "testScanner.h"
#include <iostream>
#include <iomanip>
using namespace std;

string allTokens[] = {
   //Id and Integer Tokens
   "IdTk", "IntTk",
   //Keyword Tokens
   "StartTk","StopTk","IterateTk","VoidTk","VarTk","ReturnTk","InTk","OutTk","ProgramTk","CondTk","ThenTk","LetTk",
   //Symbol Tokens
   "EqualsTk","LessThanTk","GreaterThanTk","LessThanEqualTk","GreaterThanEqualTk","EqualEqualTk","ColonTk","PlusTk",
   "MinusTk","MultiplicationTk","DivideTk","PercentTk","PeriodTk","LeftParenthesesTk","RightParenthesesTk",
   "CommaTk","LeftBraceTk","RightBraceTk","SemiColonTk","RightSquareBrackTk","LeftSquareBrackTk",
   //End of file and error Tokens
   "EofTk","ErrorTk"
};

void testScanner(ifstream& inputValues){
   //Default token 
   Token currToken = Token();
   //Initialize the line number in the file to 1
   int lineNum = 1;
   //Do-while to loop through the tokens while there is input values 
   do{
      Token currToken = scanner(inputValues, lineNum);
      if (currToken.identiToken == ErrorTk){
         //use exit_failure because this is void
         exit(EXIT_FAILURE);
      }
      else{
         tokenOutput(currToken);
      }
   } while(inputValues);
   
   return;
}

//Display the resulting tokens, line numbers, and the input
void tokenOutput(Token token)
{
   cout << "Line Number: " << token.lineNum;
   cout << "\t\tInput Value: " << setw(9) << token.stringToken;
   cout << "\t\tToken: " << setw(18) << allTokens[token.identiToken] << endl;   
   return;
}
