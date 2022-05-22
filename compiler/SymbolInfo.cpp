#include <bits/stdc++.h>
#include "SymbolInfo.h"
using namespace std;




SymbolInfo::SymbolInfo(){this->Next=nullptr;}
SymbolInfo::SymbolInfo(string Name,string Type)
{
    this->Name=Name;
    this->Type=Type;

    Next=NULL;

}

SymbolInfo::~SymbolInfo()
{
}


