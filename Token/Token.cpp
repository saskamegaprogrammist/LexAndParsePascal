//
// Created by alexis on 04.01.20.
//

#include "Token.h"

Token::Token(const string value, const int i) {
    this->number = i;
    this->value = value;
}

string Token::GetValue() {
    return this->value;
}

int Token::GetNumber() {
    return this->number;
}