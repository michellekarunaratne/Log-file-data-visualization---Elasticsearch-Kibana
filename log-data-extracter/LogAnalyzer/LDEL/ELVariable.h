//
//  ELVariable.h
//  LogAnalyzer
//
//  Created by Dileepa Jayathilaka on 12/24/13.
//  Copyright (c) 2013 99x Eurocenter. All rights reserved.
//

#ifndef __LogAnalyzer__ELVariable__
#define __LogAnalyzer__ELVariable__

#include <iostream>
#include "CommonIncludes.h"

class ELVariable {
public:
    MSTRING name;
    ELVariable();
    virtual ~ELVariable();

    virtual bool EvaluateString (MSTRING& str, MSTRING::size_type startPos, MSTRING::size_type& newPos) = 0;
    virtual void AddNodesForEvaluatedStrings(PNODE parent);
    virtual bool IsConstant();
    
protected:
    MSTRING evaluatedStr;
    int id=0;
    int finalpos=0;
    MSTRING evaluatedStrArray[10];
};

#endif /* defined(__LogAnalyzer__ELVariable__) */
