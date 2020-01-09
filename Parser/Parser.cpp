//
// Created by alexis on 08.01.20.
//

#include "Parser.h"


#include <exception>
#include <algorithm>
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
// D : <IDENT/number/> <:=> E <;> D | <IDENT/string/> <:=> F <;> D | eps
// E : E1 E2
// E1 : E3 E4
// E2 : <+> E | <-> E | eps
// E3 : <+> E3 | <-> E3 | (E) | <VALUE> | <IDENT>
// E4 : <*> E1 | </> E1 | <mod> E1 | <div> E1 | eps

//<E> ::= <T> <E’>.
//<E’> ::= + <T> <E’> | - <T> <E’> | .
//<T> ::= <F> <T’>.
//<T’> ::= * <F> <T’> | / <F> <T’> | .
//<F> ::= <number> | <var> | ( <E> ) | - <F>.




const vector<string> NUMTYPES = {"shortint", "smallint", "integer", "longint", "int64",  "real",
                              "double", "decimal"};
const vector<string> STRTYPES = {"char", "string"};

void Parser::CreateException(int &index) {
    currentIndex = index;
    throw "error while parsing";
}

bool Parser::CheckNumType(string type) {
    return find(NUMTYPES.begin(), NUMTYPES.end(), type) != NUMTYPES.end();
}

bool Parser::CheckStringType(string type) {
    return find(STRTYPES.begin(), STRTYPES.end(), type) != STRTYPES.end();
}

bool Parser::CheckBooleanType(string type) {
    return (type == "boolean");
}

bool Parser::CheckIdentExists(string name) {
    for (int i=0; i < idents.size(); i++) {
        if (idents[i].GetName() == name) {
            currentIdent = idents[i];
            return true;
        }
    }
    return false;
}

Ident* Parser::GetIdent(string name) {
    for (int i=0; i < idents.size(); i++) {
        if (idents[i].GetName() == name) {
            return &idents[i];
        }
    }
    return NULL;
}

void Parser::CreateIdent(ParseToken &token) {
    Ident ident = Ident(token.GetValue(), idents.size());
    idents.push_back(ident);
}

bool Parser::ParseE4() {
    int index = currentIndex;
    index++;
    if (tokens.size() >= currentIndex+1+1 && tokens[index].GetValue() == "*") {
        currentIndex = index;
        if (!ParseE1()) {
            CreateException(currentIndex);
        } else {
            return true;
        }
    } else {
        if (tokens.size() >= currentIndex+1+1 && tokens[index].GetValue() == "/") {
            currentIndex = index;
            if (!ParseE1()) {
                CreateException(currentIndex);
            } else {
                return true;
            }
        } else {
            if (tokens.size() >= currentIndex+1+1 && tokens[index].GetValue() == "div") {
                currentIndex = index;
                if (!ParseE1()) {
                    CreateException(currentIndex);
                } else {
                    return true;
                }
            } else  {
                if (tokens.size() >= currentIndex+1+1 && tokens[index].GetValue() == "mod") {
                    currentIndex = index;
                    if (!ParseE1()) {
                        CreateException(currentIndex);
                    } else {
                        return true;
                    }
                } else return true;
            }
        }
    }
}

bool Parser::ParseE3() {
    if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "+") {
        if (!ParseE3()) {
            CreateException(currentIndex);
        } else {
            return true;
        }
    } else {
        if (tokens[index].GetValue() == "-") {
            if (!ParseE3()) {
                CreateException(currentIndex);
            } else {
                return true;
            }
        } else {
            if (tokens[index].GetValue() == "(") {
                currentIndex = index;
                if (!ParseE()) {
                    CreateException(currentIndex);
                } else {
                    if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
                    index=currentIndex;
                    index++;
                    if (tokens[index].GetValue() == ")") {
                        currentIndex = index;
                        return true;
                    } else CreateException(index);
                }

            } else {
                if (tokens[index].GetType() == "NUMBER") {
                    currentIndex = index;
                    return true;
                } else {
                    if (tokens[index].GetType() == "IDENT") {
                        Ident* ident = GetIdent(tokens[index].GetValue());
                        if (ident != NULL) {
                            if (CheckNumType(ident->GetType())) {
                                currentIndex=index;
                                return true;
                            } else CreateException(index);
                        } else CreateException(index);
                    } else CreateException(index);
                }
            }
        }
    }

}

bool Parser::ParseE2() {
    int index = currentIndex;
    index++;
    if (tokens.size() >= currentIndex+1+1 && tokens[index].GetValue() == "-") {
        currentIndex = index;
        if (!ParseE()) {
            CreateException(currentIndex);
        } else {
            return true;
        }
    } else {
        if (tokens.size() >= currentIndex+1+1 && tokens[index].GetValue() == "+") {
            currentIndex = index;
            if (!ParseE()) {
                CreateException(currentIndex);
            } else {
                return true;
            }
        } else {
            return true;
        }
    }
}

bool Parser::ParseE1() {
    if (!ParseE3()) {
        CreateException(currentIndex);
    } else {
        ParseE4();
    }
    return true;
}

bool Parser::ParseE() {
    if (!ParseE1()) {
        CreateException(currentIndex);
    } else {
        ParseE2();
    }
    return true;
}

bool Parser::ParseD() {
    int index = currentIndex;
    index++;
    if (tokens.size() >= currentIndex+1+2 && tokens[index].GetType() == "IDENT") {
       if (CheckIdentExists(tokens[index].GetValue())) {
           if (CheckNumType(currentIdent.GetType())) {
               index++;
               if (tokens[index].GetValue() == ":=") {
                   currentIndex = index;
                   if (ParseE()) {
                       if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
                       index = currentIndex;
                       index++;
                       if (tokens[index].GetValue() == ";") {
                           currentIndex = index;
                           return ParseD();
                       } else CreateException(index);
                   }
                   else CreateException(currentIndex);
               } else {
                   CreateException(index);
               }
           }

       } else {
           CreateException(index);
       }
    }
    else return true;
}

bool Parser::ParseC1() {
    int index = currentIndex;
    index++;
    if (tokens.size() >= currentIndex+1+1 && tokens[index].GetValue() == "begin") {
        currentIndex = index;
        if (!ParseC1()) CreateException(currentIndex);
        if (tokens.size() < currentIndex+1+2) CreateException(currentIndex);
        index = currentIndex;
        index++;
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
    if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "begin") {
        currentIndex = index;
        if (!ParseC1()) CreateException(currentIndex);
        if (tokens.size() < currentIndex+1+2) CreateException(currentIndex);
        index = currentIndex;
        index++;
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
    if (tokens.size() >= currentIndex+1+1 && tokens[index].GetType() == "IDENT") {
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
    if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == ",") {
        if (tokens.size() < currentIndex+1+2) CreateException(currentIndex);
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
        if (tokens.size() < currentIndex+1+3) CreateException(currentIndex);
        if (tokens[index].GetValue() == ":") {
            index++;
            if (tokens[index].GetType() == "TYPE") {
                for (int j=idents.size()-1; j >=0; j--) {
                    if (idents[j].GetType() == "") {
                        idents[j].SetType(tokens[index].GetValue());
                    } else break;
                }
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
    if (tokens.size() >= currentIndex+1+2 && tokens[index].GetValue() == "var") {
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
    if (tokens.size() < 3) CreateException(currentIndex);
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