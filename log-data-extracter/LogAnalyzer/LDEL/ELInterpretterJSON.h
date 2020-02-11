//
//  ELInterpretterJSON.h
//  LogAnalyzer

#ifndef __LogAnalyzer__ELInterpretterJSON__
#define __LogAnalyzer__ELInterpretterJSON__

#include <iostream>
#include "CommonIncludes.h"
#include "Node.h"

class ELInterpretterJSONResult {
public:
    PNODE startNode;
    WIDESTRING finalString;
    int millisecondsForParsing;
    int millisecondsForInterpreting;
    ELInterpretterJSONResult();
};

class ELLineAnnotationElementJSON {
public:
    MSTRING::size_type start;
    MSTRING::size_type len;
    MSTRING name;
};

class ELLineAnnotationJSON {
public:
    VEC_ELLINEANNOTATIONELEMENT_JSON elements;
};

class ELInterpretterJSON {
public:
    ELInterpretterJSONResult* EvaluateCase(MSTRING sDefFile);

private:
    WIDESTRING ProcessLinesInFile(MSTRING sLogFile, VEC_ELLINETEMPLATE& vecLineTemplates, ELInterpretterJSONResult *res);
    WIDESTRING ProcessBlocks(WIDESTRING str, VEC_ELBLOCKTEMPLATE& blockTemplates, VEC_ELLINETEMPLATE& lineTemplates, ELInterpretterJSONResult *res);
    void UnifyBlock(WIDESTRING& str, ELBlockTemplate *block, ELInterpretterJSONResult *res);
    void UnifyBlockNonDefinite(WIDESTRING& str, ELBlockTemplate *block, ELInterpretterJSONResult *res);
    void PrintInterpretterResult(ELInterpretterJSONResult *ir);
    VEC_ELBLOCKTEMPLATE GetBlockTemplatesPreparedToHandleRecursiveDefs(VEC_ELBLOCKTEMPLATE& blockTemplates);
};

#endif /* defined(__LogAnalyzer__ELInterpretterJSON__) */
