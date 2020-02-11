//
//  ELFloat.cpp
//  LogAnalyzer
//
//  Created by Dileepa Jayathilaka on 12/24/13.
//  Copyright (c) 2013 99x Eurocenter. All rights reserved.
//

#include "ELJSON.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

ELJSON::ELJSON()
        : ELVariable() {
}

ELJSON::~ELJSON() {

}

bool ELJSON:: EvaluateString (MSTRING& str, MSTRING::size_type startPos, MSTRING::size_type& newPos) {
    MSTRING::size_type pos = startPos;
    bool succ = false;
    //MSTRING nums = "0123456789";

    int openParenthesisCount=0;
    int closeParenthesisCount=0;

    MSTRING stringEvaluatedSoFar = EMPTY_STRING;
    while (true) {
        if (pos == MSTRING::npos) {
            break;
        }
        MCHAR ch = str.at(pos);
        stringEvaluatedSoFar+=ch;
        if( ch =='{'){
            //break;
            openParenthesisCount++;
        }
        if(ch =='}'){
            closeParenthesisCount++;

        }
        if(openParenthesisCount<=closeParenthesisCount)
        {
            newPos=++pos;
            break;
        }
        ++pos;
        if(pos>=str.size()){
            break;
        }
    }

    try
    {
        json j1=json::parse(stringEvaluatedSoFar);
        evaluatedStr = stringEvaluatedSoFar;
//        evaluatedStrArray[id]=stringEvaluatedSoFar;
//        ++id;
        succ=true;
    }
    catch (exception& e)
    {
        cout << "Standard exception: " << e.what() << endl;
        exit (EXIT_FAILURE);
    }

    return succ;
}
