#include "Tests.h"
#include "DefFileReader.h"
#include "ScriptReader.h"
#include "MetaData.h"
#include "ExecutionTemplateList.h"
#include "ExecutionContext.h"
#include "Node.h"
#include "MemMan.h"


void Tests::RunTest1()
{
	DefFileReader dfr;
    MetaData* pMD = dfr.Read("../Core/TestCases/files/test1/Defs.txt");
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
    MetaData* pMD = dfr.Read("../Core/TestCases/files/test2/Defs.txt");
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
    MetaData* pMD = dfr.Read("../Core/TestCases/files/test3/Defs.txt");
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

void Tests::RunTest4()
{
	DefFileReader dfr;
    MetaData* pMD = dfr.Read("../Core/TestCases/files/test5/Defs.txt");
	ScriptReader sr;
	ScriptReaderOutput op;
	bool bSucc = sr.ProcessScript(pMD, op, "$X.GetAggregatedValue.WriteToFile(/Users/MurtazaA/99X/Backend/MurtazaQLVersion/FlexibleComputerLanguage/FlexibleComputerLanguage/report_Unix)");
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

void Tests::RunTest5()
{
    std::wcout<<"Test 5 Started\n";
	DefFileReader dfr;
    MetaData* pMD = dfr.Read("../Core/TestCases/files/test5/Defs.txt");
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
    std::cout <<"X : "<< pX->GetValue()<<"\n";
    std::cout <<"Y : "<< pY->GetValue()<<"\n";
	pX->DestroyWithSubTree();
	pY->DestroyWithSubTree();
	pZ->DestroyWithSubTree();
}
void Tests::RunTest6()
{
    DefFileReader dfr;
    MetaData* pMD = dfr.Read("D:/MurtazaCode/FlexibleComputerLanguage/Core/TestCases/files/TestLoadFromCodeLibraryScriptDefs.txt");
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
    //std::cout <<"X : "<< pX->GetValue()<<"\n";
    // std::cout <<"Y : "<< pY->GetValue()<<"\n";
    std::cout <<"Z : "<< pZ->GetChildCount()<<"\n";
    pX->DestroyWithSubTree();
    pY->DestroyWithSubTree();
    pZ->DestroyWithSubTree();
}

