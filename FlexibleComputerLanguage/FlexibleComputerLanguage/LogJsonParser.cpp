//
// Created by AfazD on 31-Oct-19.
//

#include "LogJsonParser.h"
#include "MemMan.h"
#include "Node.h"
#include "MetaData.h"
#include "Strings.h"
#include "Entity.h"
#include "easylogging++.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "json.hpp"

using json = nlohmann::json;

Node *LogJsonParser::LogJSONToNodeTree(std::string jsonString)
{
    int id=0;
    rapidjson::Document logs;
    logs.Parse(jsonString.c_str());
    Node *root = MemoryManager::Inst.CreateNode(++id);
    root->SetCustomString("root");
    for(rapidjson::Value::ConstValueIterator itr = logs.Begin(); itr != logs.End(); ++itr){

        for(rapidjson::Value::ConstMemberIterator tps = itr->MemberBegin(); tps != itr->MemberEnd(); ++tps) {
            Node* currentNode=MemoryManager::Inst.CreateNode(++id);
            //std::cout << tps->name.GetString() << "\n";
            currentNode->SetValue((char *) tps->name.GetString());
            char* val=(char *)tps->name.GetString();
            currentNode->SetCustomString(val);
            rapidjson::Value &jsonvalue = (rapidjson::Value &) (itr->GetObjectA()[tps->name.GetString()]);
            currentNode=LogJsonParser::LOGJSONToNodeTreeRecursively(jsonvalue,currentNode);
            root->AppendNode(currentNode);
        }

    }

    return root;
}

Node* LogJsonParser::LOGJSONToNodeTreeRecursively(rapidjson::Value &j,Node* parent){

    int id=0;
    for(rapidjson::Value::ConstMemberIterator data=j.MemberBegin(); data!=j.MemberEnd();data++){
        rapidjson::Value &jsonvalue = j[data->name.GetString()];
        if(jsonvalue.IsObject()){
            //std::cout<<"\t"<<data->name.GetString()<<"\n";
            Node *dataNode=MemoryManager::Inst.CreateNode(++id);
            dataNode->SetValue((char *) data->name.GetString());
            char* val=(char *)data->name.GetString();
            dataNode->SetCustomString(val);
            parent->AppendNode(dataNode);
            LOGJSONToNodeTreeRecursively(jsonvalue,dataNode);
        }
        else{
            //std::cout<<"\t not object : "<<data->name.GetString()<<"\n";
            Node *dataNode=MemoryManager::Inst.CreateNode(++id);
            dataNode->SetValue((char *) data->name.GetString());
            char* val=(char *)data->name.GetString();
            dataNode->SetCustomString(val);
            Node *valueNode=MemoryManager::Inst.CreateNode(++id);
            valueNode->SetValue((char*)(data->value.GetString()));
            char* val1=(char *)data->value.GetString();
            valueNode->SetCustomString(val1);
            dataNode->AppendNode(valueNode);
            parent->AppendNode(dataNode);
        }
    }

    return parent;
}

void LogJsonParser::LogNodeTreetoJson(Node* node)
{
    std::ofstream newjsonfile;
    newjsonfile.open("../FlexibleComputerLanguage/maskedJSON.json");
    Node* curr =node->GetFirstChild();
    newjsonfile<<_MSTR([{)<<_MSTR(\n);
    bool hasChild =false;
    while (curr)
    {
        PrintNodeToFile(newjsonfile,curr,hasChild);
        if(curr->GetRightSibling())
        {
            newjsonfile <<"},\n";
        }
        curr = curr->GetRightSibling();
    }
    newjsonfile<<_MSTR(\n)<<_MSTR(}]);
    newjsonfile.close();
}

void LogJsonParser::PrintNodeToFile(std::ofstream &newjsonfile,Node* node,int count)
{
    MSTRING nodeValue = node->GetValue();
    json j1=nodeValue;
    if(node->GetChildCount()>0)
    {
        if(nodeValue=="Value")
        {
            newjsonfile <<j1 << _MSTR(:);
        }
        else
        {
            newjsonfile <<j1 << _MSTR(:)<<_MSTR({\n);
        }
    }
    else
    {
        newjsonfile <<j1<<_MSTR(\n);
    }

    PNODE child = node->GetFirstChild();
    bool hasChildren = false;
    while (child != NULL)
    {
        hasChildren = true;
        PrintNodeToFile(newjsonfile, child,count);
        if(child->GetRightSibling())
        {
            newjsonfile <<"},\n";
        }
        child = child->GetRightSibling();
    }
}