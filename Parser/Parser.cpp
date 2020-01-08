//
// Created by alexis on 08.01.20.
//

#include "Parser.h"

#include <exception>
#include <iostream>

//GRAMMAR PASCAL
//
// A : <program> <IDENT> <;> A1 | A1
// A1 : B | C
// B : <var> <IDENT> B1
// B1 : <,> <IDENT> B3 | B2
// B2: <:=> <VALUE> <;> B4 | <:> <TYPE> <:=> <VALUE> <;> B4
// B3:  <,> <IDENT> B3 | <:> <TYPE> <;> B4
// B4: <IDENT> B1 | C
// C: <begin> <end> Z
// Z: . | eps

void Parser::CreateException() {
    throw "error while parsing";
}

bool Parser::ParseC() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "begin") {
        return true;
    } else {
        CreateException();
    }
}

bool Parser::ParseB4() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetType() == "IDENT") {
        currentIndex = index;
        ParseB1();
    } else {
        if (ParseC()) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}

bool Parser::ParseB3() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == ",") {
        index++;
        if (tokens[index].GetType() == "IDENT")  {
            currentIndex = index;
            ParseB3();
        } else {
            CreateException();
        }
    } else {
        if (tokens[index].GetValue() == ":") {
            index++;
            if (tokens[index].GetType() == "TYPE") {
                index++;
                if (tokens[index].GetValue() == ";") {
                    currentIndex = index;
                    ParseB4();
                } else {
                    CreateException();
                }
            } else {
                CreateException();
            }
        } else {
            CreateException();
        }
    }
}

bool Parser::ParseB2() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == ":=") {
        index++;
        if (tokens[index].GetType() == "NUMBER") { //VALUE
            index++;
            if (tokens[index].GetValue() == ";") {
                currentIndex = index;
                ParseB4();
            } else {
                CreateException();
            }
        } else {
            CreateException();
        }
    } else {
        if (tokens[index].GetValue() == ":") {
            index++;
            if (tokens[index].GetType() == "TYPE") {
                index++;
                if (tokens[index].GetValue() == ":=") {
                    index++;
                    if (tokens[index].GetType() == "NUMBER") { //VALUE
                        index++;
                        if (tokens[index].GetValue() == ";") {
                            currentIndex = index;
                            ParseB4();
                        } else {
                            CreateException();
                        }
                    } else {
                        CreateException();
                    }

                } else {
                    CreateException();
                }
            } else {
                CreateException();
            }
        } else {
            CreateException();
        }
    }
}

bool Parser::ParseB1() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == ",") {
        index++;
        if (tokens[index].GetType() == "IDENT") {
            currentIndex = index;
            ParseB3();
        } else {
            CreateException();
        }
    } else {
        ParseB2();
    }
}


bool Parser::ParseB() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "var") {
        index++;
        if (tokens[index].GetType() == "IDENT") {
            currentIndex = index;
            ParseB1();
        }
    } else {
        return false;
    }
}


bool Parser::ParseA1() {
    if (!ParseB()) {
        if (!ParseC()) {
            CreateException();
        }
    }
    return true;
}


bool Parser::ParseA() {
    currentIndex = 0;
    if (tokens[currentIndex].GetValue() == "program") {
        currentIndex++;
        if (tokens[currentIndex].GetType() == "IDENT") {
            currentIndex++;
            if (tokens[currentIndex].GetValue() == ";") {
                ParseA1();
            } else {
                CreateException();
            }
        } else {
            CreateException();
        }
    } else ParseA1();
}

bool Parser::Parse() {
    try {
        ParseA();
    } catch (const char* e) {
        cout << e << endl;
        cout << currentIndex << endl;
        return false;
    }
}


void Parser::SetTokens(vector<ParseToken> tokens) {
    this->tokens = tokens;
}