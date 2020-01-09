#include <fstream>
#include <iostream>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Token/Token.h"
#define FILE_NAME_IN "../TestPrograms/second.txt"
using namespace std;

int main() {
    ifstream fin(FILE_NAME_IN);
    basic_string<char > testprogramm ((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    Lexer lexer;
    Parser parser;
    lexer.SetProgramm(testprogramm);
    if (lexer.Normalize()) {
        vector<char> norm = lexer.GetNormalizedVector();
        for (int i = 0; i < norm.size(); i++) {
            std::cout << norm[i] << std::endl;
        }
    }
    if (lexer.Tokenize()) {
        vector<Token> tokens = lexer.GetTokens();
        for (int i = 0; i < tokens.size(); i++) {
            std::cout << tokens[i].GetValue() << " " << tokens[i].GetNumber() << std::endl;
        }
        vector<Token> operators = lexer.GetOperators();
        for (int i = 0; i < operators.size(); i++) {
            std::cout << operators[i].GetValue() << " " << operators[i].GetNumber() << std::endl;
        }
        vector<Token> operations = lexer.GetOperations();
        for (int i = 0; i < operations.size(); i++) {
            std::cout << operations[i].GetValue() << " " << operations[i].GetNumber() << std::endl;
        }
        vector<Token> strings = lexer.GetStrings();
        for (int i = 0; i < strings.size(); i++) {
            std::cout << strings[i].GetValue() << " " << strings[i].GetNumber() << std::endl;
        }
        vector<ParseToken> parseTokens = lexer.GetParseTokens();
        for (int i = 0; i < parseTokens.size(); i++) {
            std::cout << parseTokens[i].GetNumber() << " " << parseTokens[i].GetValue() << " " << parseTokens[i].GetType() << std::endl;
        }
        parser.SetTokens(parseTokens);
        parser.Parse();
    vector<Ident> idents = parser.GetIdents();
    for (int i = 0; i < idents.size(); i++) {
        std::cout << idents[i].GetValue() << " " << idents[i].GetNumber() << " " << idents[i].GetType()<< " " << idents[i].GetName() << std::endl;
    }
    }

    return 0;
}
