//
// Created by AfazD on 10-Oct-19.
//

#ifndef __LogAnalyzer__ELJSON__
#define __LogAnalyzer__ELJSON__

#include <iostream>
#include "ELVariable.h"

class ELJSON : public ELVariable {
public:
    ELJSON();
    virtual ~ELJSON();
    bool EvaluateString (MSTRING& str, MSTRING::size_type startPos, MSTRING::size_type& newPos);
};


#endif //LOGANALYZER_ELJSON_H
