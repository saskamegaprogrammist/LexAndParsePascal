#include <iostream>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Token/Token.h"
using namespace std;

const string testProgramm = "program HelloWorld;\n"
                            "var\n"
                            "        m, n: integer ;\n"
                            "        t: string ;\n"
                            "        r: char ;\n"
                            "begin\n"
                            "m:= 7 mod (+2);\n"
                            "r:= 'h';\n"
                            "z:= 'hello' + 'hiii' + r;\n"
                            "end.";


int main() {
    Lexer lexer;
    Parser parser;
    lexer.SetProgramm(testProgramm);
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
//        vector<Token> operators = lexer.GetOperators();
//        for (int i = 0; i < operators.size(); i++) {
//            std::cout << operators[i].GetValue() << " " << operators[i].GetNumber() << std::endl;
//        }
//        vector<Token> operations = lexer.GetOperations();
//        for (int i = 0; i < operations.size(); i++) {
//            std::cout << operations[i].GetValue() << " " << operations[i].GetNumber() << std::endl;
//        }
//        vector<Token> strings = lexer.GetStrings();
//        for (int i = 0; i < strings.size(); i++) {
//            std::cout << strings[i].GetValue() << " " << strings[i].GetNumber() << std::endl;
//        }
        vector<ParseToken> parseTokens = lexer.GetParseTokens();
        for (int i = 0; i < parseTokens.size(); i++) {
            std::cout << parseTokens[i].GetNumber() << " " << parseTokens[i].GetValue() << " " << parseTokens[i].GetType() << std::endl;
        }
        parser.SetTokens(parseTokens);
        parser.Parse();
//    vector<Ident> idents = parser.GetIdents();
//    for (int i = 0; i < idents.size(); i++) {
//        std::cout << idents[i].GetValue() << " " << idents[i].GetNumber() << " " << idents[i].GetType()<< " " << idents[i].GetName() << std::endl;
//    }
    }

    return 0;
}

//"program HelloWorld;\n"
//"var\n"
//"        n, i: integer;\n"
//"begin\n"
//"        n := 3;\n"
//"n *= 3;\n"
//"writeln(n);\n"
//"for i:= 1 to 10 do\n"
//"begin\n"
//"        writeln(i);\n"
//"end;\n"
//"end."
//
//"program FirstProgramm;\n"
//"var i: integer; (* sdfsdf*)\n"
//"begin\n"
//"//here should be comment\n"
//"    ddsf\n"
//"end\n";
