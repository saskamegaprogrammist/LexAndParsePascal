//
// Created by alexis on 08.01.20.
//

#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include <vector>
#include "../ParseToken/ParseToken.h"

using namespace std;

class Parser {
private:
    int currentIndex;
    vector<ParseToken> tokens;
    bool ParseA();
    bool ParseA1();
    bool ParseB();
    bool ParseB1();
    bool ParseB2();
    bool ParseB3();
    bool ParseB4();
    bool ParseC();
    void CreateException();

public:
    Parser() = default;
    void SetTokens(vector<ParseToken> tokens);
    bool Parse();
};


#endif //LEXER_PARSER_H
