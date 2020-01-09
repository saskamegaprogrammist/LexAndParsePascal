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
// C1 : <begin> C1 <end> <;> | D | I
// D : <IDENT/number/> <:=> E <;> D | <IDENT/string/> <:=> F <;> D | <IDENT/boolean/> <:=> G <;> D | <IDENT/char/> <:=> H <;> D | eps
// E : E1 E2
// E1 : E3 E4
// E2 : <+> E | <-> E | eps
// E3 : <+> E3 | <-> E3 | (E) | <NUMBER> | <IDENT/number/>
// E4 : <*> E1 | </> E1 | <mod> E1 | <div> E1 | eps
// F : F1 F2
// F1: (F) | <STRING> | <IDENT/string or char/>
// F2 : <+> F | eps
// G : (G) | <BOOLEAN> | <IDENT/boolean/>
// H : (H) | <STRING/length==1/> | <IDENT/char/>
// I : <for> I1
// I1 : <IDENT/number/> <:=> <NUMBER> I2
// I2 : <to> <NUMBER> <do> | <downto> <NUMBER> <do>

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
    return (type == "string");
}

bool Parser::CheckCharType(string type) {
    return (type == "char");
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


bool Parser::ParseH() {
    if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "(") {
        currentIndex = index;
        if (!ParseH()) {
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
        if (tokens[index].GetType() == "STRING" && tokens[index].GetValue().length() == 3) {
            currentIndex = index;
            return true;
        } else {
            if (tokens[index].GetType() == "IDENT") {
                Ident* ident = GetIdent(tokens[index].GetValue());
                if (ident != NULL) {
                    if (CheckCharType(ident->GetType())) {
                        currentIndex=index;
                        return true;
                    } else CreateException(index);
                } else CreateException(index);
            } else CreateException(index);
        }
    }
}


bool Parser::ParseG() {
    if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "(") {
        currentIndex = index;
        if (!ParseG()) {
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
        if (tokens[index].GetType() == "BOOLEAN") {
            currentIndex = index;
            return true;
        } else {
            if (tokens[index].GetType() == "IDENT") {
                Ident* ident = GetIdent(tokens[index].GetValue());
                if (ident != NULL) {
                    if (CheckBooleanType(ident->GetType())) {
                        currentIndex=index;
                        return true;
                    } else CreateException(index);
                } else CreateException(index);
            } else CreateException(index);
        }
    }
}


bool Parser::ParseF2() {
    int index = currentIndex;
    index++;
    if (tokens.size() >= currentIndex+1+1 && tokens[index].GetValue() == "+") {
        currentIndex = index;
        if (!ParseF()) {
            CreateException(currentIndex);
        } else {
            return true;
        }
    } else {
        return true;
    }
}

bool Parser::ParseF1() {
    if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
    int index = currentIndex;
    index++;
    if (tokens[index].GetValue() == "(") {
        currentIndex = index;
        if (!ParseF()) {
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
        if (tokens[index].GetType() == "STRING") {
            currentIndex = index;
            return true;
        } else {
            if (tokens[index].GetType() == "IDENT") {
                Ident* ident = GetIdent(tokens[index].GetValue());
                if (ident != NULL) {
                    if (CheckStringType(ident->GetType()) || CheckCharType(ident->GetType())) {
                        currentIndex=index;
                        return true;
                    } else CreateException(index);
                } else CreateException(index);
            } else CreateException(index);
        }
    }
}

bool Parser::ParseF() {
    if (!ParseF1()) {
        CreateException(currentIndex);
    } else {
        ParseF2();
    }
    return true;
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
        currentIndex = index;
        if (!ParseE3()) {
            CreateException(currentIndex);
        } else {
            return true;
        }
    } else {
        if (tokens[index].GetValue() == "-") {
            currentIndex = index;
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
           } else {
               if (CheckStringType(currentIdent.GetType())) {
                   index++;
                   if (tokens[index].GetValue() == ":=") {
                       currentIndex = index;
                       if (ParseF()) {
                           if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
                           index = currentIndex;
                           index++;
                           if (tokens[index].GetValue() == ";") {
                               currentIndex = index;
                               return ParseD();
                           } else CreateException(index);
                       }
                       else CreateException(currentIndex);
                   } else CreateException(index);
               } else {
                   if (CheckBooleanType(currentIdent.GetType())) {
                       index++;
                       if (tokens[index].GetValue() == ":=") {
                           currentIndex = index;
                           if (ParseG()) {
                               if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
                               index = currentIndex;
                               index++;
                               if (tokens[index].GetValue() == ";") {
                                   currentIndex = index;
                                   return ParseD();
                               } else CreateException(index);
                           }
                           else CreateException(currentIndex);
                       } else CreateException(index);
                   } else {
                       if (CheckCharType(currentIdent.GetType())) {
                           index++;
                           if (tokens[index].GetValue() == ":=") {
                               currentIndex = index;
                               if (ParseH()) {
                                   if (tokens.size() < currentIndex+1+1) CreateException(currentIndex);
                                   index = currentIndex;
                                   index++;
                                   if (tokens[index].GetValue() == ";") {
                                       currentIndex = index;
                                       return ParseD();
                                   } else CreateException(index);
                               }
                               else CreateException(currentIndex);
                           } else CreateException(index);
                       } else CreateException(index);
                   }
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
            idents[idents.size()-1].SetType("funcname");
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
        else {
            cout << "THIS IS PASCAL PROGRAM" << endl;
        }
    } catch (const char* e) {
        cout << e << endl;
        cout << currentIndex << endl;
        return false;
    }
}


void Parser::SetTokens(vector<ParseToken> tokens) {
    this->tokens = tokens;
}

vector<Ident> Parser::GetIdents() {
    return this->idents;
}