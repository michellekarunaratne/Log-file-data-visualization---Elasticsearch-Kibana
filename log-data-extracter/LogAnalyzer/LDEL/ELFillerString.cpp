//
//  ELFillerString.cpp
//  LogAnalyzer
//
//  Created by Dileepa Jayathilaka on 12/24/13.
//  Copyright (c) 2013 99x Eurocenter. All rights reserved.
//

#include "ELFillerString.h"
#include <iostream>
using namespace std;

ELFillerString::ELFillerString()
: ELVariable() {
    
}

ELFillerString::~ELFillerString() {
    
}

void ELFillerString::AddChar(MCHAR ch) {
    characters.insert(ch);
}

bool ELFillerString::EvaluateString (MSTRING& str, MSTRING::size_type startPos, MSTRING::size_type& newPos) {
    MSTRING::size_type pos = startPos;
    MSTRING::size_type strlen = str.length();
    MSTRING stringEvaluatedSoFar = EMPTY_STRING;
    bool succ = false;
    while (true) {
        if (pos == MSTRING::npos) {
            break;
        }
        if (pos >= strlen) {
            break;
        }
        MCHAR ch = str.at(pos);
        if (characters.find(ch) != characters.end()) {
            ++pos;
            succ = true;
            newPos = (pos);
            stringEvaluatedSoFar += ch;
        }
        else {
            break;
        }
    }

    if (succ) {

        //std::cout<<id<<" : "<<stringEvaluatedSoFar<<"\n";
        evaluatedStr = stringEvaluatedSoFar;
//        if(evaluatedStrArray[id].length()!=0 && evaluatedStrArray[id]==stringEvaluatedSoFar && id!=0){
//            evaluatedStrArray[++id]=stringEvaluatedSoFar;
//        }
//        if(evaluatedStrArray[id].length()!=0 && evaluatedStrArray[id]!=stringEvaluatedSoFar){
//            evaluatedStrArray[++id]=stringEvaluatedSoFar;
//        } else{
//            evaluatedStrArray[id]=stringEvaluatedSoFar;
//        }

    }
    /*if(succ== false){
        cout<< "Standard exception in Filler String"<< endl;
        exit(EXIT_FAILURE);
    }
*/
    return succ;
}
