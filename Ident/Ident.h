//
// Created by alexis on 08.01.20.
//

#ifndef LEXER_IDENT_H
#define LEXER_IDENT_H

#include <string>

using namespace std;


class Ident {
private:
    string value;
    string type;
    string name;
    int number;
public:
    Ident() = default;
    Ident(const string name, const int number);
    void SetValue(string value);
    void SetType(string type);
    void SetName(string name);
    string GetValue();
    string GetType();
    string GetName();
    int GetNumber();
};


#endif //LEXER_IDENT_H
