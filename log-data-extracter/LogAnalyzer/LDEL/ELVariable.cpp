//
//  ELVariable.cpp
//  LogAnalyzer
//
//  Created by Dileepa Jayathilaka on 12/24/13.
//  Copyright (c) 2013 99x Eurocenter. All rights reserved.
//

#include "ELVariable.h"
#include "Node.h"
#include "ELNodeWrapper.h"

ELVariable::ELVariable() {
    name = EMPTY_STRING;
    evaluatedStr = EMPTY_STRING;
}

ELVariable::~ELVariable() {
    
}
void ELVariable::AddNodesForEvaluatedStrings(PNODE parent) {

//    std::cout<<sizeof(evaluatedStrArray)/ sizeof(evaluatedStrArray[0])<<"\n";
//    std::cout<<"Start of Array\n";
//    int len=sizeof(evaluatedStrArray)/ sizeof(evaluatedStrArray[0]);
//    for(int i=0;i<len;i++){
//        std::cout<<evaluatedStrArray[i]<<"\n";
//    }
//   std::cout<<"End of Array\n\n";
    ELNodeWrapperInfo info;
    info.type = ELNODE_TYPE_VARIABLE;
    info.name = name;
//    if(evaluatedStrArray[finalpos].length()==0){
//        finalpos--;
//    }
    //info.value = evaluatedStrArray[finalpos++];
    info.value = evaluatedStr;
    //std::cout<<evaluatedStr<<"\n";
    info.parserElement = this;
    evaluatedStr = EMPTY_STRING;
    ELNodeWrapper *wrapper = new ELNodeWrapper(info);
    PNODE newNode = wrapper->GetNode();
    parent->AppendNode(newNode);
}

bool ELVariable::IsConstant() {
    return false;
}