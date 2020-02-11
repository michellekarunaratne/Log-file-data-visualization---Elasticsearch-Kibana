//
//  ELNodeWrapperJSON.h
//  LogAnalyzer


#ifndef __LogAnalyzer__ELNodeWrapperJSON__
#define __LogAnalyzer__ELNodeWrapperJSON__

#include <iostream>
#include "CommonIncludes.h"
#include "Node.h"

const MBYTE ELNODE_TYPE_VARIABLE = 0;
const MBYTE ELNODE_TYPE_VARIABLE_SEQUENCE = 1;
const MBYTE ELNODE_TYPE_LINE = 2;
const MBYTE ELNODE_TYPE_LINE_SEQUENCE = 3;
const MBYTE ELNODE_TYPE_BLOCK = 4;
const MBYTE ELNODE_TYPE_BLOCK_SEQUENCE = 5;

class ELVariable;

class ELNodeWrapperJSONInfo {
public:
    MBYTE type;
    MSTRING name;
    MSTRING value;
    PVOID parserElement;    // ELVariable, ELLineTemplate or ELBlockTemplate that defines this node.
};

class ELNodeWrapperJSON {
public:
    static MAP_NODE_WRAPPER_JSON mapNodeToWrapper;
    ELNodeWrapperJSON(ELNodeWrapperJSONInfo& info);
    virtual ~ELNodeWrapperJSON();
    PNODE GetNode();
    void PrintNodeToFile(MOFSTREAM& o,int count);
    
private:
    static MULONG nodeId;
    
    ELNodeWrapperJSON();
    void PrintNodeToFile(MOFSTREAM& jsonfile, PNODE theNode, int tabCount,int count);
    void StartNewLine(MOFSTREAM& jsonfile, int tabCount);
    
protected:
    Node* node;
};

#endif /* defined(__LogAnalyzer__ELNodeWrapperJSON__) */
