//
// Created by alexis on 08.01.20.
//

#ifndef LEXER_PARSETOKEN_H
#define LEXER_PARSETOKEN_H

#include <string>

using namespace std;

class ParseToken {
private:
    string value;
    string type;
public:
    ParseToken() = default;
    ParseToken(const string value, const string type);
    string GetValue();
    string GetType();
};

#endif //LEXER_PARSETOKEN_H
