//Chase Richards
//CMPSCI 4280 P3
//Filename: parser.cpp

#include "parser.h"
#include "scanner.h"
#include <iostream>
using namespace std;

//input file
ifstream inFile;
//initilize the infile line number
int lineNum = 1;
//Current token in
Token tkn;

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

//Parser function called from main.cpp before printing tree
Node* parser(){
   Node* node; //root node
   tkn = scanner(inFile, lineNum); //begin scanning
   node = program(); //program is the first <non-terminal> in our BNF so call it
   //if it is the EofTk then we are finished and it was successful
   if(tkn.identiToken == EofTk){
      //cout << "Parsing Complete" << endl;
      return node; //root node
   } 
   //if we receive a token other than eof that is unexpected
   else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: EofTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
}

//<program>-><vars><block>
Node* program(){
   int treeDepth = 0;
   //<program> node
   Node* node = new Node("<program>", treeDepth);   
   //the two children are <vars> and <block>
   node -> firstChild = vars(treeDepth);
   node -> secondChild = block(treeDepth);
   return node;
}

//<block> -> start<vars><stats>stop
Node* block(int treeDepth){
   treeDepth++;
   Node* node = new Node("<block>", treeDepth);
   //start<vars><stats>stop check
   if(tkn.identiToken == StartTk){
      tkn = scanner(inFile, lineNum);
      node -> firstChild = vars(treeDepth);
      node -> secondChild = stats(treeDepth);
      if(tkn.identiToken == StopTk){
         tkn = scanner(inFile, lineNum);
         return node;
      }else{
         cout << "Parsing Error on line: " << lineNum << "\nToken Expected: StopTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
         exit(EXIT_FAILURE);
      }
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: StartTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
}

//<vars> -> empty|var identifier : Integer <vars>
Node* vars(int treeDepth){
   treeDepth++;   
   Node* node = new Node("<vars>", treeDepth);
   //Check each token
   if(tkn.identiToken == VarTk){
      tkn = scanner(inFile, lineNum);
      if(tkn.identiToken == IdTk){
         //need to store IdTk
         node -> firstToken = tkn;
         tkn = scanner(inFile, lineNum);
         if(tkn.identiToken == ColonTk){
            tkn = scanner(inFile, lineNum);
            if(tkn.identiToken == IntTk){
               //need to store IntTk
               node -> secondToken = tkn;
               tkn = scanner(inFile, lineNum);
               //vars is the first child
               node -> firstChild = vars(treeDepth);
               return node;
            }else{
               cout << "Parsing Error on line: " << lineNum << "\nToken Expected: IntTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
               exit(EXIT_FAILURE);
            }
         }else{
            cout << "Parsing Error on line: " << lineNum << "\nToken Expected: ColonTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
            exit(EXIT_FAILURE);
         }
      }else{
         cout << "Parsing Error on line: " << lineNum << "\nToken Expected: IdTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
         exit(EXIT_FAILURE);
      }
   }else{
      //empty production
      return nullptr;
   }
}

//<expr> -> <A> + <expr>| <A>
Node* expr(int treeDepth){
   treeDepth++;
   Node* node = new Node("<expr>", treeDepth);
   node -> firstChild = A(treeDepth);
   if(tkn.identiToken == PlusTk){
      //store the PlusTk
      node -> firstToken = tkn;
      tkn = scanner(inFile, lineNum);
      node -> secondChild = expr(treeDepth);
      return node;
   }else{
      return node;
   }
}

//<A> -> <N>-<A>|<N>
Node* A(int treeDepth){
   treeDepth++;
   Node* node = new Node("<A>", treeDepth);
   node -> firstChild = N(treeDepth);
   if(tkn.identiToken == MinusTk){
      //store the PlusTk
      node -> firstToken = tkn;
      tkn = scanner(inFile, lineNum);
      node -> secondChild = A(treeDepth);
      return node;
   }else{
      return node;
   }
}

//<N>-><M>/<N>|<M>*<N>|<M>
Node* N(int treeDepth){
   treeDepth++;
   Node* node = new Node("<N>", treeDepth);
   node -> firstChild = M(treeDepth);
   if(tkn.identiToken == DivideTk){
      node -> firstToken = tkn;
      tkn = scanner(inFile, lineNum);
      node -> secondChild = N(treeDepth);
      return node;
   }else if(tkn.identiToken == MultiplicationTk){
      node -> firstToken = tkn;
      tkn = scanner(inFile, lineNum);
      node -> secondChild = N(treeDepth);
      return node;
   }else{
      return node;
   }
}

//<M> -> -<M>|<R>
Node* M(int treeDepth){
   treeDepth++;
   Node* node = new Node("<M>", treeDepth);
   if(tkn.identiToken == MinusTk){
      node -> firstToken = tkn;
      tkn = scanner(inFile, lineNum);
      node -> firstChild = M(treeDepth);
      return node;
   }else{
      node -> firstChild = R(treeDepth);
      return node;
   }
}

//<R> -> [<expr]|identifier|integer
Node* R(int treeDepth){
   treeDepth++;
   Node* node = new Node("<R>", treeDepth);
   if(tkn.identiToken == LeftSquareBrackTk){
      tkn = scanner(inFile, lineNum);
      node -> firstChild = expr(treeDepth);
      if(tkn.identiToken == RightSquareBrackTk){
         tkn = scanner(inFile, lineNum);
         return node;
      }else{
         cout << "Parsing Error on line: " << lineNum << "\nToken Expected: RightSquareBrackTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
         exit(EXIT_FAILURE);
      }
   }else if(tkn.identiToken == IdTk){
      node -> firstToken = tkn;
      tkn = scanner(inFile, lineNum);
      return node;
   }else if(tkn.identiToken == IntTk){
      node -> firstToken = tkn;
      tkn = scanner(inFile, lineNum);
      return node;
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: LeftSquareBrackTk, IdTk or IntTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
}

//<stats> -> <stat>;<mstat>
Node* stats(int treeDepth){
   treeDepth++;
   Node* node = new Node("<stats>", treeDepth);
   node -> firstChild = stat(treeDepth);
   if(tkn.identiToken == SemiColonTk){
      tkn = scanner(inFile, lineNum);
      node -> secondChild = mstat(treeDepth);
      return node;
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: SemiColonTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
   return node;
}

//<mStat> -> empty|<stat>;<mStat>
Node* mstat(int treeDepth){
   treeDepth++;
   Node* node = new Node("<mStat>", treeDepth);
   if(tkn.identiToken == InTk || tkn.identiToken == OutTk || tkn.identiToken == StartTk || tkn.identiToken == CondTk || tkn.identiToken == IterateTk || tkn.identiToken == IdTk){
      node -> firstChild = stat(treeDepth);
      if(tkn.identiToken == SemiColonTk){
         tkn = scanner(inFile, lineNum);
         node -> secondChild = mstat(treeDepth);
         return node;
      }
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: InTk, OutTk, StartTk, CondTk, IterateTk or IdTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }else{
      //empty production
      return nullptr;
   }
}

//<stat> -> <in>|<out>|<block>|<if>|<loop>|<assign>
Node* stat(int treeDepth){
   treeDepth++;
   Node* node = new Node("<stat>", treeDepth);
   //first production of <in>
   if(tkn.identiToken == InTk){
      node -> firstChild = in(treeDepth);
      return node;
   }else if(tkn.identiToken == OutTk){ //first production of <out>
      node -> firstChild = out(treeDepth);
      return node;
   }else if(tkn.identiToken == StartTk){ //first production of <block>
      node -> firstChild = block(treeDepth);
      return node;
   }else if(tkn.identiToken == CondTk){ //first production of <if>
      node -> firstChild = iff(treeDepth);
      return node;
   }else if(tkn.identiToken == IterateTk){ //first production of <loop>
      node -> firstChild = loop(treeDepth);
      return node;
   }else if(tkn.identiToken == IdTk){ //first production of <assign>
      node -> firstChild = assign(treeDepth);
      return node;
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: InTk, OutTk, StartTk, CondTk, IterateTk, or IdTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
}

//<in> -> in Identifier
Node* in(int treeDepth){
   treeDepth++;
   Node* node = new Node("<in>", treeDepth);
   if(tkn.identiToken == InTk){
      tkn = scanner(inFile, lineNum);
      if(tkn.identiToken == IdTk){
         node -> firstToken = tkn;
         tkn = scanner(inFile, lineNum);
         return node;
      }else{
         cout << "Parsing Error on line: " << lineNum << "\nToken Expected: IdTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
         exit(EXIT_FAILURE);
      }
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: InTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
}

//<out> -> out<expr>
Node* out(int treeDepth){
   treeDepth++;
   Node* node = new Node("<out>", treeDepth);
   if(tkn.identiToken == OutTk){
      tkn = scanner(inFile, lineNum);
      node -> firstChild = expr(treeDepth);
      return node;
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: OutTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
}

//<if> -> cond((<expr><RO><expr>))<stat>
Node* iff(int treeDepth){
   treeDepth++;
   Node* node = new Node("<if>", treeDepth);
   if(tkn.identiToken == CondTk){
      tkn = scanner(inFile, lineNum);
      if(tkn.identiToken == LeftParenthesesTk){
         tkn = scanner(inFile, lineNum);
         if(tkn.identiToken == LeftParenthesesTk){
            tkn = scanner(inFile, lineNum);
            node -> firstChild = expr(treeDepth);
            node -> secondChild = RO(treeDepth);
            node -> thirdChild = expr(treeDepth);
            if(tkn.identiToken == RightParenthesesTk){
               tkn = scanner(inFile, lineNum);
               if(tkn.identiToken == RightParenthesesTk){
                  tkn = scanner(inFile, lineNum);
                  node -> fourthChild = stat(treeDepth);
                  return node;
               }else{
                  cout << "Parsing Error on line: " << lineNum << "\nToken Expected: RightParenthesesTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
                  exit(EXIT_FAILURE);
               }
            }else{
               cout << "Parsing Error on line: " << lineNum << "\nToken Expected: RightParenthesesTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
               exit(EXIT_FAILURE);
            }
         }else{
            cout << "Parsing Error on line: " << lineNum << "\nToken Expected: LeftParenthesesTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
            exit(EXIT_FAILURE);
         }
      }else{
         cout << "Parsing Error on line: " << lineNum << "\nToken Expected: LeftParenthesesTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
         exit(EXIT_FAILURE);
      }
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: CondTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
}


//<loop> -> iterate((<expr><RO><expr>)) <stat>
Node* loop(int treeDepth){
   treeDepth++;
   Node* node = new Node("<loop>", treeDepth);
   if(tkn.identiToken == IterateTk){
      tkn = scanner(inFile, lineNum);
      if(tkn.identiToken == LeftParenthesesTk){
         tkn = scanner(inFile, lineNum);
         if(tkn.identiToken == LeftParenthesesTk){
            tkn = scanner(inFile, lineNum);
            node -> firstChild = expr(treeDepth);
            node -> secondChild = RO(treeDepth);
            node -> thirdChild = expr(treeDepth);
            if(tkn.identiToken == RightParenthesesTk){
               tkn = scanner(inFile, lineNum);
               if(tkn.identiToken == RightParenthesesTk){
                  tkn = scanner(inFile, lineNum);
                  node -> fourthChild = stat(treeDepth);
                  return node;
               }else{
                  cout << "Parsing Error on line: " << lineNum << "\nToken Expected: RightParenthesesTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
                  exit(EXIT_FAILURE);
               }
            }else{
               cout << "Parsing Error on line: " << lineNum << "\nToken Expected: RightParenthesesTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
               exit(EXIT_FAILURE);
            }
         }else{
            cout << "Parsing Error on line: " << lineNum << "\nToken Expected: LeftParenthesesTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
            exit(EXIT_FAILURE);
         }
      }else{
         cout << "Parsing Error on line: " << lineNum << "\nToken Expected: LeftParenthesesTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
         exit(EXIT_FAILURE);
      }
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: IterateTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
}

//<assign> -> Identifier < < <expr>
Node* assign(int treeDepth){
   treeDepth++;
   Node* node = new Node("<assign>", treeDepth);
   if(tkn.identiToken == IdTk){
      //store IdTk
      node -> firstToken = tkn;
      tkn = scanner(inFile, lineNum);
      if(tkn.identiToken == LessThanTk){
         //store LessThanTk
         node -> secondToken = tkn;
         tkn = scanner(inFile, lineNum);
         if(tkn.identiToken == LessThanTk){
            //store second LessThanTk
            node -> thirdToken = tkn;
            tkn = scanner(inFile, lineNum);
            //expr non terminal
            node -> firstChild = expr(treeDepth);
            return node;
         }else{
            cout << "Parsing Error on line: " << lineNum << "\nToken Expected: LessThanTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
            exit(EXIT_FAILURE);
         }
      }else{
         cout << "Parsing Error on line: " << lineNum << "\nToken Expected: LessThanTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
         exit(EXIT_FAILURE);
      }
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: IdTk\nToken Received: " << allTokens[tkn.identiToken] << endl;
      exit(EXIT_FAILURE);
   }
}


//<RO> -> <|<<|>|>>|=|<>
Node* RO(int treeDepth){
   treeDepth++;
   Node* node = new Node("<RO>", treeDepth);
   //if the token is <, >, = store the token and get the next
   if(tkn.identiToken == LessThanTk || tkn.identiToken == GreaterThanTk || tkn.identiToken == EqualsTk){
      node -> firstToken = tkn;
      tkn = scanner(inFile, lineNum);
      //Cover the cases of <<, >>, <> and store it because the first and second one are seperate tokens
      if(tkn.identiToken == LessThanTk || tkn.identiToken == GreaterThanTk){
         node -> secondToken = tkn;
         tkn = scanner(inFile, lineNum);
         return node;
      }else{
         return node;
      }
   }else{
      cout << "Parsing Error on line: " << lineNum << "\nToken Expected: LessThanTk, GreaterThanTk, or EqualsTk\n Token Received: " << tkn.identiToken << endl;
      exit(EXIT_FAILURE);
   }
}

