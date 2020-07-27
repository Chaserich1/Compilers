//Chase Richards
//CMPSCI 4280 P3
//Filename: token.h

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

//Enum for all of the valid tokens
enum idTokens{
   //Identifier and Integer Tokens
   IdTk,IntTk,
   //Keyword Tokens
   StartTk,StopTk,IterateTk,VoidTk,VarTk,ReturnTk,InTk,OutTk,ProgramTk,CondTk,ThenTk,LetTk,
   //Symbol Tokens
   EqualsTk,LessThanTk,GreaterThanTk,LessThanEqualTk,GreaterThanEqualTk,EqualEqualTk,ColonTk,
   PlusTk,MinusTk,MultiplicationTk,DivideTk,PercentTk,PeriodTk,LeftParenthesesTk,RightParenthesesTk,
   CommaTk,LeftBraceTk,RightBraceTk,SemiColonTk,RightSquareBrackTk,LeftSquareBrackTk,
   //End of file and error Tokens
   EofTk,ErrorTk
};

//Struct Token
struct Token{
   idTokens identiToken;
   string stringToken;
   int lineNum;

   //default constr token starts as an error but can change, if it doesn't change then it is an error
   Token(){
      this -> identiToken = ErrorTk;
      this -> stringToken = "";
      this -> lineNum = 0;
   }

   //Token
   Token(idTokens identiToken, string stringToken, int lineNum){
      this -> identiToken = identiToken;
      this -> stringToken = stringToken;
      this -> lineNum = lineNum;
   }
};

#endif
