//
// Created by alexis on 04.01.20.
//

#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H


#include <vector>
#include "../Token/Token.h"
#include "../ParseToken/ParseToken.h"

class Lexer {

private:
    int currentIndex;
    string programm;
    vector<char> normalizedVector;
    vector<Token> idents;
    vector<Token> keywords;
    vector<Token> operators;
    vector<Token> operations;
    vector<Token> numbers;
    vector<Token> types;
    vector<Token> tokens;
    vector<ParseToken> parseTokens;
    bool CheckComment(int ind);
    void DeleteComment(int ind);
    bool CheckSpace(int ind);
    void DeleteSpace(int ind);
    bool CheckKeyword(int i);
    bool CheckType(int i);
    bool CheckOperator(int i);
    bool CheckOperation(int i);
    bool CheckIdent(int i);
    bool CheckNumber(int i);
    bool CheckLetter(const char c);
    bool CheckDigit(const char d);
public:
    Lexer() = default;
    void SetProgramm(const string programm);
    bool Normalize();
    bool Tokenize();
    vector<char> GetNormalizedVector();
    vector<Token> GetTokens();
    vector<ParseToken> GetParseTokens();
    vector<Token> GetNumbers();
    vector<Token> GetTypes();
    vector<Token> GetOperators();
    vector<Token> GetOperations();
    vector<Token> GetKeywords();
    vector<Token> GetIdents();
};


#endif //LEXER_LEXER_H
