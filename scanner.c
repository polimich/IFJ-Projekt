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
  NumberExponent,
  NumberExponentSign,
  NumberDouble,
  NumberExponentFinal,
  Semicolon,
  Comma,
  LPar,
  RPar,
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
  Point, 
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
        if(edge == ')') return RPar;
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

    //Number state
    case Number:
      if(isdigit(edge)) return Number;
      else if(edge == '.') return Point;
      else if(edge == 'e' || edge == 'E') return NumberExponent;
      return Error;
    case Point:
      if(isdigit(edge)) return NumberDouble;
      return Error;
    case NumberDouble:
      if(isdigit(edge)) return NumberDouble;
      else if(edge == 'e' || edge == 'E') return NumberExponent;
      return Error;    
    case NumberExponent:
      if(isdigit(edge)) return NumberExponentFinal;
      else if(edge == '+' || edge == '-') return NumberExponentSign;
      return Error;
    case NumberExponentSign:
      if(isdigit(edge)) return NumberExponentFinal;
      return Error;
    case NumberExponentFinal:
      if(isdigit(edge)) return NumberExponentFinal;
      return Error;
    
    //Identifier or Keyword
    case Identifier:
      if (isalpha(edge) || edge == '_') return Identifier;
      return Error;

    //single char operators
    case Semicolon:
    case Comma:
    case LPar:
    case RPar:
    case Div:
    case Mul:
    case Plus:
    case Minus:
    
  }
}
