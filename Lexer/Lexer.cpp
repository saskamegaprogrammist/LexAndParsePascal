//
// Created by alexis on 04.01.20.
//

#include <exception>
#include <iostream>
#include <algorithm>
#include "Lexer.h"

using namespace std;

const char RETURN = 10;
const char SPACE = 32;
const char LEFTBRACKET = 40;
const char RIGHTBRACKET = 41;
const char STAR = 42;
const char SLASH = 47;
const char LEFTCURLYBRACKET = 123;
const char RIGHTCURLYBRACKET = 125;
const char FIRSTNUMBER = 48;
const char LASTNUMBER = 57;
const char FIRSTLETTER = 97;
const char LASTLETTER= 122;
const char FIRSTCAPLETTER = 65;
const char LASTCAPLETTER= 90;
const char UNDERLINE = 95;
const char QUOTE = 39;
const vector<string> BOOLEANS = {"true", "false"};
const vector<string> KEYWORDS = {"program"};
const vector<string> OPERATIONS = {"+", "-", "*", "/", "(", ")", "@", "not", "^", "div", "mod", "and", "shl", "shr", "or", "xor", "=",
                                   ">", "<", "<>", "<=", ">=", "as", "is", "in", "new"};
const vector<string> OPERATORS = {"begin", "end", "for", "to", ":", ".", ",", ":=", ";", "+=", "*=", "-=", "/=", "if", "then", "else",
                                  "var", "case", "of", "downto", "loop", "do", "foreach", "in", "while", "do", "repeat", "until",
                                  "break", "continue", "exit", "try", "except", "finally"};
const vector<string> TYPES = {"shortint", "smallint", "integer", "longint", "int64", "byte", "word", "longword", "cardinal", "real",
                        "double", "single", "decimal", "boolean", "char", "string", "array", "record"};

bool Lexer::CheckLetter (const char letter) {
    return ((letter <= LASTLETTER && letter >= FIRSTLETTER) ||
            (letter <= LASTCAPLETTER && letter >= FIRSTCAPLETTER));
}

bool Lexer::CheckDigit (const char digit) {
    return (digit <= LASTNUMBER && digit >= FIRSTNUMBER);
}

bool Lexer::CheckType(int index) {
    string type = "";
    type += normalizedVector[index];
    int i = index + 1;
    for (; i < normalizedVector.size(); i++) {
        if (CheckLetter(normalizedVector[i])) {
            type += normalizedVector[i];
        } else break;
    }

    if (find(TYPES.begin(), TYPES.end(), type) != TYPES.end()) {
        Token token = Token(type, types.size());
        types.push_back(token);
        Token token_n = Token("TYPE", types.size()-1);
        tokens.push_back(token_n);
        ParseToken token_p = ParseToken(type, "TYPE");
        parseTokens.push_back(token_p);
        currentIndex = i;
        return true;
    } else {
        return false;
    }
}

bool Lexer::CheckKeyword(int index) {
    string keyword = "";
    keyword += normalizedVector[index];
    int i = index + 1;
    for (; i < normalizedVector.size(); i++) {
        if (CheckLetter(normalizedVector[i])) {
            keyword += normalizedVector[i];
        } else break;
    }

    if (find(KEYWORDS.begin(), KEYWORDS.end(), keyword) != KEYWORDS.end()) {
        Token token = Token(keyword, keywords.size());
        keywords.push_back(token);
        Token token_n = Token("KEYWORD", keywords.size()-1);
        tokens.push_back(token_n);
        ParseToken token_p = ParseToken(keyword, "KEYWORD");
        parseTokens.push_back(token_p);
        currentIndex = i;
        return true;
    } else {
        return false;
    }
}

bool Lexer::CheckOperator(int index) {
    string op = "";
    string trueop = "";
    op += normalizedVector[index];
    int i = index;
    if (find(OPERATORS.begin(), OPERATORS.end(), op) != OPERATORS.end()) {
        trueop = op;
        currentIndex = index + 1;
    }
    i++;
    if (CheckLetter(normalizedVector[index])) {
        for (; i < normalizedVector.size(); i++) {
            if (CheckLetter(normalizedVector[i])) {
                op += normalizedVector[i];
                if (find(OPERATORS.begin(), OPERATORS.end(), op) != OPERATORS.end()) {
                    trueop = op;
                    currentIndex = i+1;
                }
            } else break;
        }
    } else {
        for (; i < normalizedVector.size(); i++) {
            if (normalizedVector[i] != SPACE) {
                op += normalizedVector[i];
                if (find(OPERATORS.begin(), OPERATORS.end(), op) != OPERATORS.end()) {
                    trueop = op;
                    currentIndex = i+1;
                }
            } else break;
        }
    }
    if (trueop != "") {
        Token token = Token(trueop, operators.size());
        operators.push_back(token);
        Token token_n = Token("OPERATOR", operators.size()-1);
        tokens.push_back(token_n);
        ParseToken token_p = ParseToken(trueop, "OPERATOR");
        parseTokens.push_back(token_p);
        return true;
    } else {
        return false;
    }
}

bool Lexer::CheckIdent(int index) {
    string ident = "";
    ident += normalizedVector[index];
    int i = index + 1;
    for (; i < normalizedVector.size(); i++) {
        if (CheckDigit(normalizedVector[i]) || CheckLetter(normalizedVector[i]) ||
            normalizedVector[i] == UNDERLINE) {
            ident += normalizedVector[i];
        } else break;
    }
    Token token = Token(ident, idents.size());
    idents.push_back(token);
    Token token_n = Token("IDENT", idents.size()-1);
    tokens.push_back(token_n);
    ParseToken token_p = ParseToken(ident, "IDENT");
    parseTokens.push_back(token_p);
    currentIndex = i;
    return true;
}

bool Lexer::CheckString(int index) {
    string string = "";
    string += normalizedVector[index];
    int i = index + 1;
    for (; i < normalizedVector.size(); i++) {
        if (normalizedVector[i]!=QUOTE) {
            string += normalizedVector[i];
        } else {
            string += normalizedVector[i];
            i++;
            break;
        }
    }
    if (i == normalizedVector.size()) {
        return false;
    } else {
        Token token = Token(string, strings.size());
        strings.push_back(token);
        Token token_n = Token("STRING", strings.size()-1);
        tokens.push_back(token_n);
        ParseToken token_p = ParseToken(string, "STRING");
        parseTokens.push_back(token_p);
        currentIndex = i;
        return true;
    }
}

bool Lexer::CheckBoolean(int index) {
    string boolean = "";
    boolean += normalizedVector[index];
    int i = index + 1;
    for (; i < normalizedVector.size(); i++) {
        if (CheckLetter(normalizedVector[i])) {
            boolean += normalizedVector[i];
        } else break;
    }

    if (find(BOOLEANS.begin(), BOOLEANS.end(), boolean) != BOOLEANS.end()) {
        Token token = Token(boolean, booleans.size());
        booleans.push_back(token);
        Token token_n = Token("BOOLEAN", booleans.size()-1);
        tokens.push_back(token_n);
        ParseToken token_p = ParseToken(boolean, "BOOLEAN");
        parseTokens.push_back(token_p);
        currentIndex = i;
        return true;
    } else {
        return false;
    }
}

bool Lexer::CheckNumber(int index) {
    string number = "";
    number += normalizedVector[index];
    int i = index + 1;
    for (; i < normalizedVector.size(); i++) {
        if (CheckDigit(normalizedVector[i])) {
            number += normalizedVector[i];
        } else break;
    }
    Token token = Token(number, numbers.size());
    numbers.push_back(token);
    Token token_n = Token("NUMBER", numbers.size()-1);
    tokens.push_back(token_n);
    ParseToken token_p = ParseToken(number, "NUMBER");
    parseTokens.push_back(token_p);
    currentIndex = i;
    return true;
}


bool Lexer::CheckOperation(int index) {
    string op = "";
    string trueop = "";
    op += normalizedVector[index];
    int i = index;
    if (find(OPERATIONS.begin(), OPERATIONS.end(), op) != OPERATIONS.end()) {
        trueop = op;
        currentIndex = index+1;
    }
    i++;
    for (; i < normalizedVector.size(); i++) {
        if (normalizedVector[i] != SPACE) {
            op += normalizedVector[i];
            if (find(OPERATIONS.begin(), OPERATIONS.end(), op) != OPERATIONS.end()) {
                trueop = op;
                currentIndex = i+1;
            }
        } else break;
    }
    if (trueop != "") {
        Token token = Token(trueop, operations.size());
        operations.push_back(token);
        Token token_n = Token("OPERATION", operations.size()-1);
        tokens.push_back(token_n);
        ParseToken token_p = ParseToken(trueop, "OPERATION");
        parseTokens.push_back(token_p);
        return true;
    } else {
        return false;
    }
}

bool Lexer::CheckSpace(int index) {
    if (programm[index] == SPACE || programm[index] == RETURN){
        return true;
    }
    return false;
}

void Lexer::DeleteSpace(int index) {
    for (int i = index + 1; i < programm.size(); i++) {
        if (programm[i] != SPACE) {
            currentIndex=i;
            return;
        }
    }
    currentIndex = index+1;
    return;
}

bool Lexer::CheckComment(int index) {
    if (programm[index] == LEFTBRACKET) {
        if (index+1 == programm.length()) {
            throw "not a paskal programm";
        }
        if (programm[index+1] == STAR) {
            return true;
        } else return false;
    }
    if (programm[index] == LEFTCURLYBRACKET) {
        return true;
    }
    if (programm[index] == SLASH) {
        if (index+1 == programm.length()) {
            throw "not a paskal programm";
        }
        if (programm[index+1] == SLASH) {
            return true;
        }
        return false;
    }
    return false;
}

void Lexer::DeleteComment(int index) {
    bool flag;
    flag = false;
    if (programm[index] == LEFTBRACKET) {
        for (int i = index + 2; i < programm.size(); i++) {
            if (programm[i] == STAR) {
                if (programm[i + 1] == RIGHTBRACKET) {
                    currentIndex = i + 2;
                    flag = true;
                    break;
                }
            }
        }
    }
    if (programm[index] == LEFTCURLYBRACKET) {
        for (int i = index + 2; i < programm.size(); i++) {
            if (programm[i] == RIGHTCURLYBRACKET) {
                currentIndex = i + 1;
                flag = true;
                break;
            }
        }
    }
    if (programm[index] == SLASH) {
        flag = true;
        for (int i = index + 2; i < programm.size(); i++) {
            if (programm[i] == RETURN) {
                if (normalizedVector.at(normalizedVector.size()-1) != SPACE) {
                    normalizedVector.push_back(SPACE);
                }
                currentIndex = i + 1;
                break;
            }
        }
    }
    if (!flag) {
        throw "not a paskal programm";
    }
}

bool Lexer::Normalize() {
    currentIndex=0;
    for (int i=0; i < programm.size(); ) {
        try {
            if (CheckComment(i)) {
                DeleteComment(i);
                i = currentIndex;
            } else {
                if (CheckSpace(i)) {
                    if (normalizedVector.size()==0 || normalizedVector.at(normalizedVector.size() - 1) != SPACE) {
                        normalizedVector.push_back(SPACE);
                    }
                    DeleteSpace(i);
                    i = currentIndex;
                } else {
                    normalizedVector.push_back(programm[i]);
                    i++;
                }
            }
        } catch (const char* e) {
            cout << e << endl;
            cout << currentIndex << endl;
            return false;
        }
    }
    return true;
}

bool Lexer::Tokenize() {
    currentIndex=0;
    for (int i=0; i<normalizedVector.size();) {
        try {
            if (normalizedVector[i] == SPACE) {
                i++;
            } else {
                if (CheckLetter(normalizedVector[i])) {
                    if (!CheckKeyword(i)){
                        if (!CheckType(i)) {
                            if (!CheckBoolean(i)) {
                                if (!CheckOperator(i)) {
                                    if (!CheckOperation(i)) {
                                        if (!CheckIdent(i)) {
                                            throw "error while tokenizing";
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else {
                    if (normalizedVector[i] == UNDERLINE) {
                        if(!CheckIdent(i)){
                            throw "error while tokenizing";
                        }
                    } else {
                        if (CheckDigit(normalizedVector[i])) {
                            if (!CheckNumber(i)) {
                                throw "error while tokenizing";
                            }
                        } else {
                            if (normalizedVector[i] == QUOTE) {
                                if (!CheckString(i)) {
                                    throw "error while tokenizing";
                                }
                            } else {
                                if (!CheckOperator(i)) {
                                    if (!CheckOperation(i)) {
                                        throw "error while tokenizing";
                                    }
                                }
                            }
                        }
                    }
                }
                i=currentIndex;
            }
        } catch (const char* e) {
            cout << e << endl;
            cout << currentIndex << endl;
            return false;
        }
    }
    return true;
}

void Lexer::SetProgramm(const string programm){
    this->programm = programm;
}

vector<char> Lexer::GetNormalizedVector() {
    return this->normalizedVector;
}

vector<ParseToken> Lexer::GetParseTokens() {
    return this->parseTokens;
}

vector<Token> Lexer::GetTokens() {
    return this->tokens;
}
vector<Token> Lexer::GetTypes() {
    return this->types;
}

vector<Token> Lexer::GetIdents() {
    return this->idents;
}

vector<Token> Lexer::GetOperations() {
    return this->operations;
}

vector<Token> Lexer::GetOperators() {
    return this->operators;
}

vector<Token> Lexer::GetNumbers() {
    return this->numbers;
}

vector<Token> Lexer::GetStrings() {
    return this->strings;
}

vector<Token> Lexer::GetBooleans() {
    return this->booleans;
}

vector<Token> Lexer::GetKeywords() {
    return this->keywords;
}