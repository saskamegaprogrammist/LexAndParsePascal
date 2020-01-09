//
// Created by alexis on 08.01.20.
//

#ifndef LEXER_PARSETOKEN_H
#define LEXER_PARSETOKEN_H

#include <string>

using namespace std;

class ParseToken {
private:
    int number;
    string value;
    string type;
public:
    ParseToken() = default;
    ParseToken(const int number, const string value, const string type);
    int GetNumber();
    string GetValue();
    string GetType();
};

#endif //LEXER_PARSETOKEN_H
