#include "Tests.h"
#include "DefFileReader.h"
#include "ScriptReader.h"
#include "MetaData.h"
#include "ExecutionTemplateList.h"
#include "ExecutionContext.h"
#include "Node.h"
#include "MemMan.h"
#include "ELInterpretter.h"
#include "ELInterpretterJSON.h"
#include <iostream>

using namespace std;

void Tests::RunTest1()
{
	DefFileReader dfr;
	MetaData* pMD = dfr.Read("../tests/test1/Defs.txt");
	ScriptReader sr;
	ScriptReaderOutput op;
	bool bSucc = sr.ProcessScript(pMD->s_RuleFileName, pMD, op);
	if(!bSucc)
	{
		std::wcout<<"\nFailed to read script\n";
	}
	ExecutionContext ec;
	ec.p_mapFunctions = &op.map_Functions;
	ec.p_MD = pMD;
	Node* pX = MemoryManager::Inst.CreateNode(1);
	Node* pY = MemoryManager::Inst.CreateNode(2);
	ec.map_Var["X"] = pX;
	ec.map_Var["Y"] = pY;
	op.p_ETL->Execute(&ec);
}

void Tests::RunTest2()
{
	DefFileReader dfr;
	MetaData* pMD = dfr.Read("../tests/test3/Defs.txt");
	ScriptReader sr;
	ScriptReaderOutput op;
	bool bSucc = sr.ProcessScript(pMD->s_RuleFileName, pMD, op);
	if(!bSucc)
	{
		std::wcout<<"\nFailed to read script\n";
	}
	ExecutionContext ec;
	ec.p_mapFunctions = &op.map_Functions;
	ec.p_MD = pMD;
	Node* pX = MemoryManager::Inst.CreateNode(1);
	Node* pY = MemoryManager::Inst.CreateNode(2);
	Node* pZ = MemoryManager::Inst.CreateNode(3);
	ec.map_Var["X"] = pX;
	ec.map_Var["Y"] = pY;
	ec.map_Var["Z"] = pZ;
	op.p_ETL->Execute(&ec);
	pX->DestroyWithSubTree();
	pY->DestroyWithSubTree();
	pZ->DestroyWithSubTree();
	op.p_ETL->Destroy();
}

void Tests::RunTest3()
{
	DefFileReader dfr;
	MetaData* pMD = dfr.Read("../tests/test3/Defs.txt");
	ScriptReader sr;
	ScriptReaderOutput op;
	bool bSucc = sr.ProcessScript(pMD->s_RuleFileName, pMD, op);
	if(!bSucc)
	{
		std::wcout<<"\nFailed to read script\n";
	}
	ExecutionContext ec;
	ec.p_mapFunctions = &op.map_Functions;
	ec.p_MD = pMD;
	Node* pX = MemoryManager::Inst.CreateNode(1);
	Node* pY = MemoryManager::Inst.CreateNode(2);
	Node* pZ = MemoryManager::Inst.CreateNode(3);
	ec.map_Var["X"] = pX;
	ec.map_Var["Y"] = pY;
	ec.map_Var["Z"] = pZ;
	op.p_ETL->Execute(&ec);
	pX->DestroyWithSubTree();
	pY->DestroyWithSubTree();
	pZ->DestroyWithSubTree();
//	op.p_ETL->Destroy();
}

void Tests::RunLDELtest1() {
    MSTRING stringFlag;
    /*cout<<"Enter the type of file you want to generate the result in"<<std::endl;
    cout<<"(Enter \"txt\" for text file or \"json\" for json file) :\t";
    cin>>stringFlag;

    std::for_each(stringFlag.begin(), stringFlag.end(), [](char & c) {
        c = ::tolower(c);
    });

    if(stringFlag =="txt")
    {
        ELInterpretter intp;
        intp.EvaluateCase("../tests/LDEL_test1/Defs.txt");
    }
   else if(stringFlag =="json")
    {
        ELInterpretterJSON intp;
        intp.EvaluateCase("../tests/LDEL_test1/Defs.txt");
    }
   else
    {
        cout<<"Invalid file type"<<std::endl;
        exit(1);
    }*/

    ELInterpretterJSON intp;
    intp.EvaluateCase("../tests/LDEL_test1/Defs.txt");

}

void Tests::RunLDELtest2() {
    ELInterpretter intp;
    intp.EvaluateCase("../tests/LDEL_test2/Defs.txt");
}

