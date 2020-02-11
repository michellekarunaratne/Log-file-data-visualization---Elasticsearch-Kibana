//
// Created by Michelle on 10/17/2019.
//
#include "ELLogFileFilter.h"
#include "MetaData.h"

using namespace std;

void ELLogFileFilter::FilterLogFile()
{
    MFSTREAM inputFile;
    MFSTREAM outputFile;

    inputFile.open("../tests/LDEL_test1/log.txt",ios::in);
    outputFile.open("../tests/LDEL_test1/filteredLog.txt",ios::out);

    string currentLine;
    string previousLine="";
    string currentSubstring;
    string previousSubstring;
    bool checkingSubString=false;
    while(getline(inputFile,currentLine))
    {
        if(currentLine!=previousLine){
            outputFile<<currentLine<<"\n";
        }
        previousLine=currentLine;
    }

    inputFile.close();
    outputFile.close();
}
