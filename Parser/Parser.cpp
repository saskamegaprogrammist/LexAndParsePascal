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
// B1 : <,> <IDENT> B1 | <:> <TYPE> <;> B2
// B2 :  <IDENT> B1 | A1
// C : <begin> C1 <end> <.>
// C1 : <begin> C1 <end> <;> | D
// D : <IDENT> <:=> E <;> D | eps
// E : E1 E2
// E1 : E3 E4
// E2 : <+> E1 E2 | <-> E1 E2 | eps
// E3 : + E3 | - E3 | (E) | <VALUE> | <IDENT>
// E4 : <*> E3 E4 | </> E3 E4 | <mod> E3 E4 | <div> E3 E4 | eps

//<E> ::= <T> <E’>.
//<E’> ::= + <T> <E’> | - <T> <E’> | .
//<T> ::= <F> <T’>.
//<T’> ::= * <F> <T’> | / <F> <T’> | .
//<F> ::= <number> | <var> | ( <E> ) | - <F>.




const vector<string> NUMTYPES = {"shortint", "smallint", "integer", "longint", "int64",  "real",
                              "double", "decimal"};

void Parser::CreateException(int &index) {
    currentIndex = index;
    throw "error while parsing";
}

void Parser::CreateIdent(ParseToken &token) {
    Ident ident = Ident(token.GetValue(), idents.size());
    idents.push_back(ident);
}

bool Parser::ParseD() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetType() == "IDENT") {

    }
    else return true;
}

bool Parser::ParseC1() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "begin") {
        currentIndex = index;
        if (!ParseC1()) CreateException(currentIndex);
        index = currentIndex;
        if (tokens[index].GetValue() == "end") {
            index++;
            if (tokens[index].GetValue() == ";") {
                return true;
            } else {
                CreateException(index);
            }
        } else {
            CreateException(index);
        }
    } else {
        if (!ParseD()) CreateException(currentIndex);
        else {
            return true;
        }
    }
}

bool Parser::ParseC() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "begin") {
        currentIndex = index;
        if (!ParseC1()) CreateException(currentIndex);
        index = currentIndex;
        if (tokens[index].GetValue() == "end") {
            index++;
            if (tokens[index].GetValue() == ".") {
                return true;
            } else {
                CreateException(index);
            }
        } else {
            CreateException(index);
        }
    } else {
        CreateException(index);
    }
}

bool Parser::ParseB2() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetType() == "IDENT") {
        CreateIdent(tokens[index]);
        currentIndex = index;
        if (!ParseB1()) CreateException(currentIndex);
        else {
            return true;
        }
    } else {
        if (ParseA1()) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}

bool Parser::ParseB1() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == ",") {
        index++;
        if (tokens[index].GetType() == "IDENT")  {
            CreateIdent(tokens[index]);
            currentIndex = index;
            if (!ParseB1()) CreateException(currentIndex);
            else {
                return true;
            }
        } else {
            CreateException(index);
        }
    } else {
        if (tokens[index].GetValue() == ":") {
            index++;
            if (tokens[index].GetType() == "TYPE") {
                idents[idents.size()-1].SetType(tokens[index].GetValue());
                index++;
                if (tokens[index].GetValue() == ";") {
                    currentIndex = index;
                    if (!ParseB2()) CreateException(currentIndex);
                    else {
                        return true;
                    }
                } else {
                    CreateException(index);
                }
            } else {
                CreateException(index);
            }
        } else {
            CreateException(index);
        }
    }
}



bool Parser::ParseB() {
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "var") {
        index++;
        if (tokens[index].GetType() == "IDENT") {
            CreateIdent(tokens[index]);
            currentIndex = index;
            if (!ParseB1()) CreateException(currentIndex);
            else {
                return true;
            }
        }
    } else {
        return false;
    }
}


bool Parser::ParseA1() {
    if (!ParseB()) {
        if (!ParseC()) {
            CreateException(currentIndex);
        }
    }
    return true;
}


bool Parser::ParseA() {
    currentIndex = 0;
    if (tokens[currentIndex].GetValue() == "program") {
        currentIndex++;
        if (tokens[currentIndex].GetType() == "IDENT") {
            CreateIdent(tokens[currentIndex]);
            currentIndex++;
            if (tokens[currentIndex].GetValue() == ";") {
                if (!ParseA1()) CreateException(currentIndex);
                else {
                    return true;
                }
            } else {
                CreateException(currentIndex);
            }
        } else {
            CreateException(currentIndex);
        }
    } else {
        if (!ParseA1()) CreateException(currentIndex);
        else {
            return true;
        }
    }
}

bool Parser::Parse() {
    try {
        if (!ParseA()) CreateException(currentIndex);
    } catch (const char* e) {
        cout << e << endl;
        cout << currentIndex << endl;
        return false;
    }
}


void Parser::SetTokens(vector<ParseToken> tokens) {
    this->tokens = tokens;
}