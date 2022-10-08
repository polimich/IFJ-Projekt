//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace singleton.c: xpoliv07, xlukas18      //
//    Datum: 7. 10. 2022                                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    StringLit,
    StringEsc,
    Var0,
    VarId,
    Point,
    Error
} FsmState;

FsmState transition(FsmState in, char edge)
{
    switch (in) {
    case Start:
        if (isalpha(edge) || edge == '_')
            return Identifier;
        if (isdigit(edge))
            return Number;
        if (edge == ';')
            return Semicolon;
        if (edge == ',')
            return Comma;
        if (edge == '(')
            return LPar;
        if (edge == ')')
            return RPar;
        if (edge == '\\')
            return Div;
        if (edge == '*')
            return Mul;
        if (edge == '+')
            return Plus;
        if (edge == '-')
            return Minus;
        if (edge == '=')
            return Assign;
        if (edge == '>')
            return Greater;
        if (edge == '<')
            return Lesser;
        if (edge == '/')
            return Comment;
        if (edge == '\"')
            return String;
        if (edge == '$')
            return Var0;

    //Number state
    case Number:
        if (isdigit(edge))
            return Number;
        else if (edge == '.')
            return Point;
        else if (edge == 'e' || edge == 'E')
            return NumberExponent;
        return Error;
    case Point:
        if (isdigit(edge))
            return NumberDouble;
        return Error;
    case NumberDouble:
        if (isdigit(edge))
            return NumberDouble;
        else if (edge == 'e' || edge == 'E')
            return NumberExponent;
        return Error;
    case NumberExponent:
        if (isdigit(edge))
            return NumberExponentFinal;
        else if (edge == '+' || edge == '-')
            return NumberExponentSign;
        return Error;
    case NumberExponentSign:
        if (isdigit(edge))
            return NumberExponentFinal;
        return Error;
    case NumberExponentFinal:
        if (isdigit(edge))
            return NumberExponentFinal;
        return Error;

    //Identifier or Keyword
    case Identifier:
        if (isalpha(edge) || edge == '_')
            return Identifier;
        return Error;

    case Var0:
        if (isalpha(edge) || edge == '_')
            return VarId;
        return Error;

    case VarId:
        if (isalnum(edge) || edge == '_')
            return VarId;
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
    case StringLit:

    case Assign:
        if (edge == '=')
            return Equals;
        return Assign;

    case Greater:
        if (edge = '=')
            return GreaterEven;
        return Greater;

    case Lesser:
        if (edge == '=')
            return LesserEven;
        return Lesser;

    case Comment:
        if (edge == '/')
            return LineComment;
        else if (edge == '*')
            return BlockComment;

    // string
    case String:
        if (edge < 32 || edge > 255)
            return Error;
        else if (edge == "\\")
            return StringEsc;
        else if (edge == "\"")
            return StringLit;
        return String;

    case StringEsc:
        if (edge < 32 || edge > 255)
            return Error;
        return String;
    }
}

typedef struct {
    enum {
        IDENTIFIER,
        NUM,
        NUM_DOUBLE,
        NUM_EXP,
        SEMICOLON,
        COMMA,
        LPAR,
        RPAR,
        DIV,
        MUL,
        PLUS,
        MINUS,
        ASSIGN,
        EQUALS,
        GREATER,
        GREATER_E,
        LESSER,
        LESSER_E,
        L_COMMENT,
        B_COMMENT,
        STRING_LIT,
        VARID,
        LEX_EOF
    } kind;
    size_t data;

} Lexeme;

//todo dynamicky
char a[2048] = { 0 };
char* pool_startp = &a[0];

Lexeme make_lexeme(FsmState final, char* data)
{
    switch (final) {
    case Identifier:
        return (Lexeme) { .kind = IDENTIFIER, .data = data - a };
    case Number:
        return (Lexeme) { .kind = NUM, .data = data - a };
    case NumberDouble:
        return (Lexeme) { .kind = NUM_DOUBLE, .data = data - a };
    case NumberExponentFinal:
        return (Lexeme) { .kind = NUM_EXP, .data = data - a };
    case Semicolon:
        return (Lexeme) { .kind = SEMICOLON };
    case Comma:
        return (Lexeme) { .kind = COMMA };
    case LPar:
        return (Lexeme) { .kind = LPAR };
    case RPar:
        return (Lexeme) { .kind = RPAR };
    case Div:
        return (Lexeme) { .kind = DIV };
    case Mul:
        return (Lexeme) { .kind = MUL };
    case Plus:
        return (Lexeme) { .kind = PLUS };
    case Minus:
        return (Lexeme) { .kind = MINUS };
    case Assign:
        return (Lexeme) { .kind = ASSIGN };
    case Equals:
        return (Lexeme) { .kind = EQUALS };
    case Greater:
        return (Lexeme) { .kind = GREATER };
    case GreaterEven:
        return (Lexeme) { .kind = GREATER_E };
    case Lesser:
        return (Lexeme) { .kind = LESSER };
    case LesserEven:
        return (Lexeme) { .kind = LESSER_E };
    case LineComment:
        return (Lexeme) { .kind = L_COMMENT };
    case BlockComment:
        return (Lexeme) { .kind = B_COMMENT };
    case StringLit:
        return (Lexeme) { .kind = STRING_LIT, .data = data - a };
    case VarId:
        return (Lexeme) { .kind = STRING_LIT, .data = data - a };
    case Error:
        exit(1);
    }
}

Lexeme get_lexeme()
{
    int edge;
    FsmState now = Start;
    char* lexeme_text = pool_startp;
    while (true) {
        edge = getchar();
        if (edge == EOF) {
            if (now == Start) {
                return (Lexeme) { .kind = LEX_EOF };
            }
            return make_lexeme(now, lexeme_text);
        }
        FsmState next = transition(now, edge);
        if (next == Error) {
            ungetc(edge, stdin);
            pool_startp++;
            return make_lexeme(now, lexeme_text);
        }
        *(pool_startp++) = edge;
        now = next;
    }
}

char* str_lexeme(Lexeme in)
{
    switch (in.kind) {
    case LEX_EOF:
        return "EOF";
    case IDENTIFIER:
        a + in.data;
    case NUM:
        return a + in.data;
    case NUM_DOUBLE:
        return a + in.data;
    case NUM_EXP:
        return a + in.data;
    case SEMICOLON:
        return ";";
    case COMMA:
        return ",";
    case LPAR:
        return "(";
    case RPAR:
        return ")";
    case DIV:
        return "\\";
    case MUL:
        return "*";
    case PLUS:
        return "+";
    case MINUS:
        return "-";
    case ASSIGN:
        return "=";
    case EQUALS:
        return "==";
    case GREATER:
        return ">";
    case GREATER_E:
        return ">=";
    case LESSER:
        return "<";
    case LESSER_E:
        return "<=";
    case L_COMMENT:
        return "//";
    case B_COMMENT:
        return "Block";
    case STRING_LIT:
        return a + in.data;
    case VARID:
        return a + in.data;
    }
}

int main()
{
    Lexeme l = { 0 };
    while (l.kind != LEX_EOF) {
        l = get_lexeme();
    }
}
