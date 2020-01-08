//
// Created by alexis on 08.01.20.
//

#include "Ident.h"

Ident::Ident(const string name, const int i) {
    this->number = i;
    this->name = name;
}

string Ident::GetValue() {
    return this->value;
}
string Ident::GetName() {
    return this->name;
}
string Ident::GetType() {
    return this->type;
}
int Ident::GetNumber() {
    return this->number;
}
void Ident::SetValue(string value) {
    this->value = value;
}
void Ident::SetType(string type) {
    this->type = type;
}
void Ident::SetName(string name) {
    this->name = name;
}