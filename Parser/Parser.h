//
// Created by alexis on 08.01.20.
//

#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include <vector>
#include "../ParseToken/ParseToken.h"
#include "../Ident/Ident.h"

using namespace std;

class Parser {
private:
    int currentIndex;
    Ident currentIdent;
    vector<ParseToken> tokens;
    vector<Ident> idents;
    bool ParseA();
    bool ParseA1();
    bool ParseB();
    bool ParseB1();
    bool ParseB2();
    bool ParseC();
    bool ParseC1();
    bool ParseD();
    bool ParseE();
    bool ParseE1();
    bool ParseE2();
    bool ParseE3();
    bool ParseE4();
    bool ParseF();
    bool ParseF1();
    bool ParseF2();
    bool ParseG();
    bool ParseH();
    void CreateException(int& index);
    void CreateIdent(ParseToken& ident);
    bool CheckIdentExists(string name);
    Ident* GetIdent(string name);
    bool CheckStringType(string type);
    bool CheckNumType(string type);
    bool CheckBooleanType(string type);
    bool CheckCharType(string type);


public:
    Parser() = default;
    void SetTokens(vector<ParseToken> tokens);
    vector<Ident> GetIdents();
    bool Parse();
};


#endif //LEXER_PARSER_H
