#include "ELInterpretterJSON.h"
#include "DefFileReader.h"
#include "MetaData.h"
#include "ELParser.h"
#include "ELVariable.h"
#include "ELLineTemplate.h"
#include "ELBlockTemplate.h"
#include "MemMan.h"
#include "ELNodeWrapperJSON.h"
#include "Utils.h"
#include "ELBlockElement.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <fstream>

using namespace std;
using json = nlohmann::json;

ELInterpretterJSONResult::ELInterpretterJSONResult() {
    startNode = NULL;
}

ELInterpretterJSONResult* ELInterpretterJSON::EvaluateCase(MSTRING sDefFile) {
    DefFileReader dfr;
    MetaData *md = dfr.Read(sDefFile);
    ELParser p;
    ELParserResult res;
    ELInterpretterJSONResult *ir = 0;
    MemoryManager::Inst.CreateObject(&ir);
    bool succ = p.ProcessScript(md->s_ScriptFile, md, res);

    WIDESTRING sLines = ProcessLinesInFile(md->s_LogFile, res.vec_LineTemplates, ir);
    ir->finalString = ProcessBlocks(sLines, res.vec_BlockTemplates, res.vec_LineTemplates, ir);

    PrintInterpretterResult(ir);
    return ir;
}

void ELInterpretterJSON::PrintInterpretterResult(ELInterpretterJSONResult *ir) {
    MOFSTREAM jsonfile;
    int count =1;
    jsonfile.open("../tests/LDEL_test1/resultJSON.json", std::ios::out | std::ios::trunc);
    PNODE curr = ir->startNode;
    jsonfile<<_MSTR({);
                        while (curr) {
                            ELNodeWrapperJSON* wrapper = ELNodeWrapperJSON::mapNodeToWrapper[curr];
                            wrapper->PrintNodeToFile(jsonfile,count);
                            count++;
                            if(curr->GetRightSibling())
                            {
                                jsonfile <<",";
                            }
                            curr = curr->GetRightSibling();
                        }
                        jsonfile<<_MSTR(\n);
                        jsonfile <<_MSTR(});
    jsonfile.close();
}

WIDESTRING ELInterpretterJSON::ProcessLinesInFile(MSTRING sLogFile, VEC_ELLINETEMPLATE& vecLineTemplates, ELInterpretterJSONResult *res) {
    MIFSTREAM file(sLogFile.c_str());
    MSTRING sLine;
    WIDESTRING ret = EMPTY_WIDESTRING;
    res->startNode = NULL;
    PNODE currentNode = res->startNode;
    if(file.is_open())
    {
        while(!file.eof())
        {
            getline(file, sLine);
            bool matchingLineFound = false;
            ELLineTemplate *matchingLineTemplate = NULL;
            ELNodeWrapperJSONInfo info;
            VEC_ELLINETEMPLATE::iterator ite = vecLineTemplates.begin();
            VEC_ELLINETEMPLATE::iterator iteEnd = vecLineTemplates.end();
            for ( ; ite != iteEnd; ++ite) {
                ELLineTemplate *lt = (*ite);
                if (lt->ParseLine(sLine)) {
                    ret += lt->ch;
                    matchingLineFound = true;
                    matchingLineTemplate = lt;

                    info.type = ELNODE_TYPE_LINE;
                    info.name = lt->name;
                    info.value = sLine;
                    info.parserElement = lt;
                    break;
                }
            }
            if (!matchingLineFound) {
                info.type = ELNODE_TYPE_LINE;
                info.name = _MSTR(DEFAULT_LINE_TEMPLATE);
                info.value = sLine;
                ret += ELLineTemplate::defaultChar;
            }

            ELNodeWrapperJSON *wrapper = new ELNodeWrapperJSON(info);
            PNODE newNode = wrapper->GetNode();
            if (!currentNode) {
                currentNode = newNode;
                res->startNode = currentNode;
            } else {
                currentNode->SetRightSibling(newNode);
                newNode->SetLeftSibling(currentNode);
                currentNode = newNode;
            }

            if (matchingLineTemplate) {
                matchingLineTemplate->CreateNodesForLineElements(currentNode);
            }
        }
    }
    //std::wcout<<L"\nReturn string = "<<ret;
    //WIDECOUT<<_WIDESTR(\n)<<ret<<_WIDESTR(\n);
    return ret;
}

WIDESTRING ELInterpretterJSON::ProcessBlocks(WIDESTRING str, VEC_ELBLOCKTEMPLATE& blockTemplates, VEC_ELLINETEMPLATE& lineTemplates, ELInterpretterJSONResult *res) {
    WIDESTRING availableSymbols = EMPTY_WIDESTRING;
    VEC_ELLINETEMPLATE::iterator ite1 = lineTemplates.begin();
    VEC_ELLINETEMPLATE::iterator iteEnd1 = lineTemplates.end();
    for ( ; ite1 != iteEnd1; ++ite1) {
        availableSymbols += (*ite1)->ch;
    }

    blockTemplates = GetBlockTemplatesPreparedToHandleRecursiveDefs(blockTemplates);

    VEC_ELBLOCKTEMPLATE definiteBlocks;
    VEC_ELBLOCKTEMPLATE nondefiniteBlocks;
    MAP_WIDECHAR_ELBLOCKTEMPLATE mapBlocks;
    VEC_ELBLOCKTEMPLATE::iterator ite = blockTemplates.begin();
    VEC_ELBLOCKTEMPLATE::iterator iteEnd = blockTemplates.end();
    for ( ; ite != iteEnd; ++ite) {
        ELBlockTemplate *tmp = (*ite);
        if (tmp->IsDefinite()) {
            definiteBlocks.push_back(tmp);
        } else {
            nondefiniteBlocks.push_back(tmp);
        }
        mapBlocks[tmp->ch] = tmp;
    }
    while (true) {
        //cout<<"Definite\n";
        WIDESTRING strBeforeIteration = str;
        ite = definiteBlocks.begin();
        iteEnd = definiteBlocks.end();
        for ( ; ite != iteEnd; ++ite) {
            ELBlockTemplate *tmp = (*ite);
            if (tmp->IsReadyToProcess(availableSymbols)) {
                UnifyBlock(str, tmp, res);
                availableSymbols += tmp->ch;
            }
        }
        //cout<<"Non definite\n";
        ite = nondefiniteBlocks.begin();
        iteEnd = nondefiniteBlocks.end();
        for ( ; ite != iteEnd; ++ite) {
            ELBlockTemplate *tmp = (*ite);
            if (tmp->IsReadyToProcess(availableSymbols)) {
                UnifyBlockNonDefinite(str, tmp, res);
                availableSymbols += tmp->ch;
                VEC_ELBLOCKTEMPLATE::iterator ite3 = definiteBlocks.begin();
                VEC_ELBLOCKTEMPLATE::iterator iteEnd3 = definiteBlocks.end();
                for ( ; ite3 != iteEnd3; ++ite3) {
                    //cout<<"For in for\n";
                    ELBlockTemplate *tmp3 = (*ite3);
                    if (tmp3->IsReadyToProcess(availableSymbols)) {
                        UnifyBlock(str, tmp3, res);
                        availableSymbols += tmp3->ch;
                    }
                }
            }
        }
        // If no changes occured in the string, that means we have reached the end of parsing
        if (str == strBeforeIteration) {
            return  str;
        }
    }
}
VEC_ELBLOCKTEMPLATE ELInterpretterJSON::GetBlockTemplatesPreparedToHandleRecursiveDefs(VEC_ELBLOCKTEMPLATE& blockTemplates) {
    VEC_ELBLOCKTEMPLATE ret;
    VEC_ELBLOCKTEMPLATE::iterator ite = blockTemplates.begin();
    VEC_ELBLOCKTEMPLATE::iterator iteEnd = blockTemplates.end();
    for ( ; ite != iteEnd; ++ite) {
        ELBlockTemplate *bt = (*ite);
        if (bt->isUnion) {
            VEC_BLOCKELEMENT::iterator ite2 = bt->elements.begin();
            VEC_BLOCKELEMENT::iterator iteEnd2 = bt->elements.end();
            for ( ; ite2 != iteEnd2; ++ite2) {
                ELBlockElement *be = (*ite2);
                ELBlockTemplate *newTemplate = 0;
                MemoryManager::Inst.CreateObject(&newTemplate);
                newTemplate->name = bt->name;
                newTemplate->ch = bt->ch;
                newTemplate->elements.push_back(be);
                newTemplate->isUnion = false;
                ret.push_back(newTemplate);
            }
            MemoryManager::Inst.DeleteObject(bt);
        } else {
            ret.push_back(bt);
        }
    }

    return ret;
}

void ELInterpretterJSON::UnifyBlock(WIDESTRING &str, ELBlockTemplate *block, ELInterpretterJSONResult *res) {
    WIDESTRING::size_type pos = 0;
    WIDESTRING::size_type newpos;
    while (true) {
        WIDESTRING::size_type len = str.length();
        //cout<<str<<"\n";
        if (pos == len) {
            return;
        }

        if (block->TryUnify(str, pos, newpos)) {
            // alter the node structure accordingly
            unsigned long lenAlterPart = newpos - pos;
            PNODE node = res->startNode;
            for (int i = 0; i < pos; ++i) {
                node = node->GetRightSibling();
            }
            ELNodeWrapperJSONInfo info;
            info.type = ELNODE_TYPE_BLOCK;
            info.name = block->name;
            info.value = EMPTY_STRING;
            info.parserElement = block;
            ELNodeWrapperJSON *wrapper = new ELNodeWrapperJSON(info);
            PNODE newNode = wrapper->GetNode();
            PNODE left = node->GetLeftSibling();
            if (left) {
                left->SetRightSibling(newNode);
                newNode->SetLeftSibling(left);
            } else {
                res->startNode = newNode;
            }
            for (int i = 0; i < lenAlterPart; ++i) {
                PNODE temp = node->GetRightSibling();
                node->SetLeftSibling(NULL);
                node->SetRightSibling(NULL);
                newNode->AppendNode(node);
                node = temp;
            }
            if(node!=NULL)
            {
                newNode->SetRightSibling(node);
                node->SetLeftSibling(newNode);
            }
            // alter the string
            WIDESTRING head = EMPTY_WIDESTRING;
            if (pos > 0) {
                head = str.substr(0, pos);
            }
            if (newpos == len) {
                str = (head + block->ch);
                return;
            } else {
                WIDESTRING tail = str.substr(newpos, len - newpos);
                str = (head + block->ch + tail);
                ++pos;
            }
        } else {
            ++pos;
        }
    }
}
void ELInterpretterJSON::UnifyBlockNonDefinite(WIDESTRING &str, ELBlockTemplate *block, ELInterpretterJSONResult *res) {
    WIDESTRING::size_type pos = 0;
    WIDESTRING::size_type newpos;

    ELBlockElement* startEl = block->elements.front();
    ELBlockElement* endEl =block->elements.at(block->elements.size() - 1);
    bool startSeq=startEl->IsSequence();
    bool endSeq=endEl->IsSequence();
    while (true) {
        WIDESTRING::size_type len = str.length();
        //cout<<str<<"\n";
        if (pos == len) {
            return;
        }
        if (block->TryUnify(str, pos, newpos))
        {
            // alter the node structure accordingly
            if(startSeq && endSeq || !startSeq && endSeq)
            {
                newpos-=1;
            }
            unsigned long lenAlterPart = newpos - pos;
            if(lenAlterPart==0)
            {
                break;
            }
            PNODE node = res->startNode;
            for (int i = 0; i < pos; ++i) {
                node = node->GetRightSibling();
            }
            ELNodeWrapperJSONInfo info;
            info.type = ELNODE_TYPE_BLOCK;
            info.name = block->name;
            info.value = EMPTY_STRING;
            info.parserElement = block;
            ELNodeWrapperJSON *wrapper = new ELNodeWrapperJSON(info);
            PNODE newNode = wrapper->GetNode();
            PNODE left = node->GetLeftSibling();
            if (left) {
                left->SetRightSibling(newNode);
                newNode->SetLeftSibling(left);
            } else {
                res->startNode = newNode;
            }
            for (int i = 0; i < lenAlterPart; ++i) {
                if(node->GetRightSibling())
                {
                    PNODE temp = node->GetRightSibling();
                    node->SetLeftSibling(NULL);
                    node->SetRightSibling(NULL);
                    newNode->AppendNode(node);
                    node = temp;
                }

            }
            if(node!=NULL)
            {
                newNode->SetRightSibling(node);
                node->SetLeftSibling(newNode);
            }

            // alter the string
            WIDESTRING head = EMPTY_WIDESTRING;
            if (pos > 0) {
                head = str.substr(0, pos);
            }
            if (newpos == len) {
                str = (head + block->ch);
                return;
            } else {
                WIDESTRING tail = str.substr(newpos, len - newpos);
                str = (head + block->ch + tail);
                ++pos;
            }
        }
        else {
            ++pos;
        }
    }
}