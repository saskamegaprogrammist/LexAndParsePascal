//
// Created by alexis on 08.01.20.
//

#include "ParseToken.h"

ParseToken::ParseToken(const string value, const string type) {
    this->type = type;
    this->value = value;
}

string ParseToken::GetValue() {
    return this->value;
}

string ParseToken::GetType() {
    return this->type;
}