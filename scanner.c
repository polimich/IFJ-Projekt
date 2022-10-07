//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace singleton.c: xpoliv07, xlukas18      //
//    Datum: 4. 10. 2022                                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include <ctype.h>

typedef enum {
  Start,
  Identifier,
  Number,
  NumberDouble,
  NumberExponentFinal,
  Semicolon,
  Comma,
  LPar,
  PPar,
  Div,
  Mul,
  Plus,
  Minus,
  Assign,
  Equals,
  Greater,
  GreaterEven,
  Lesser,
  LesserEven,
  Comment,
  LineComment,
  BlockComment,
  String,
  Var0,
  VarId, 
  Error
} FsmState;


FsmState transition (FsmState in, char edge) {
  switch(in) {
    case Start: 
        if(isalpha(edge) || edge == '_') return Identifier;
        if(isdigit(edge)) return Number;
        if(edge == ';') return Semicolon;
        if(edge == ',') return Comma;
        if(edge == '(') return LPar;
        if(edge == ')') return PPar;
        if(edge == '\\') return Div;
        if(edge == '*') return Mul;
        if(edge == '+') return Plus;
        if(edge == '-') return Minus;
        if(edge == '=') return Assign;
        if(edge == '>') return Greater;
        if(edge == '<') return Lesser;
        if(edge == '/') return Comment;
        if(edge == '\"') return String;
        if(edge == '$') return Var0;
  }
}
