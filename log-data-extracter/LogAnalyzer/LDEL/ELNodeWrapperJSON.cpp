//
//  ELNodeWrapperJSON.cpp
//  LogAnalyzer

#include "ELNodeWrapperJSON.h"
#include "MemMan.h"
#include "ELVariable.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;


MAP_NODE_WRAPPER_JSON ELNodeWrapperJSON::mapNodeToWrapper;
MULONG ELNodeWrapperJSON::nodeId = 1;

int count123 =0;

ELNodeWrapperJSON::ELNodeWrapperJSON() {
    node = NULL;
}

ELNodeWrapperJSON::ELNodeWrapperJSON(ELNodeWrapperJSONInfo& info) {
    node = MemoryManager::Inst.CreateNode(nodeId++);
    node->SetNature(info.type);
    node->SetValue((PMCHAR)info.value.c_str());
    node->SetCustomString((PMCHAR)info.name.c_str());
    node->SetCustomObj(info.parserElement);
    ELNodeWrapperJSON::mapNodeToWrapper[node] = this;
}

ELNodeWrapperJSON::~ELNodeWrapperJSON() {
    
}

PNODE ELNodeWrapperJSON::GetNode() {
    return node;
}

void ELNodeWrapperJSON::PrintNodeToFile(MOFSTREAM &jsonfile,int count) {
    PrintNodeToFile(jsonfile, node, 0,count);
}

void ELNodeWrapperJSON::PrintNodeToFile(MOFSTREAM &jsonfile, PNODE theNode, int tabCount, int count) {
    std::string str = std::to_string(count);
    MSTRING nodeName = theNode->GetCustomString();
    MSTRING nodeValue = theNode->GetValue();
    MBYTE nodeType = theNode->GetNature();
    MSTRING nodeTypeString = EMPTY_STRING;
    if (nodeType == ELNODE_TYPE_VARIABLE) {
        nodeTypeString = _MSTR(VARIABLE);
    } else if (nodeType == ELNODE_TYPE_VARIABLE_SEQUENCE) {
        nodeTypeString = _MSTR(VARIABLE SEQUENCE);
    } else if (nodeType == ELNODE_TYPE_LINE) {
        nodeTypeString = _MSTR(LINE);
    } else if (nodeType == ELNODE_TYPE_LINE_SEQUENCE) {
        nodeTypeString = _MSTR(LINE SEQUENCE);
    } else if (nodeType == ELNODE_TYPE_BLOCK) {
        nodeTypeString = _MSTR(BLOCK);
    } else if (nodeType == ELNODE_TYPE_BLOCK_SEQUENCE) {
        nodeTypeString = _MSTR(BLOCK SEQUENCE);
    }

    json j1 =nodeTypeString.append(str) ;
    json j2 = nodeName;
    json j3 = _MSTR(Value);
    json j4 = nodeValue;

    StartNewLine(jsonfile, tabCount);
    jsonfile <<j1 << _MSTR(\t:\t)<<_MSTR({)<<j2<< _MSTR(:);
    StartNewLine(jsonfile, tabCount);
    jsonfile<< _MSTR(\t{)<<j3 << _MSTR( :)<<j4;
    jsonfile<<_MSTR(});
    PNODE child = theNode->GetFirstChild();
    bool hasChildren = false;
    while (child != NULL) {
        hasChildren = true;
        jsonfile << "," ;
        PrintNodeToFile(jsonfile, child, tabCount + 1,count);
        child = child->GetRightSibling();
        count++;
    }
   jsonfile << _MSTR(});
}
void ELNodeWrapperJSON::StartNewLine(MOFSTREAM &jsonfile, int tabCount) {
    jsonfile<<_MSTR(\n);
   for (int i = 0; i < tabCount; ++i) {
       jsonfile<<_MSTR(\t);
    }
}
