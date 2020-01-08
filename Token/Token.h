//
// Created by alexis on 04.01.20.
//

#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <string>

using namespace std;


class Token {
private:
    string value;
    int number;
public:
    Token() = default;
    Token(const string value, const int number);
    string GetValue();
    int GetNumber();
};


#endif //LEXER_TOKEN_H
