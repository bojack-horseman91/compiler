#include "ScopeTable.h"
#include <bits/stdc++.h>
#include "SymbolInfo.h"
using namespace std;

class SymbolTable
{
private:
    stack<ScopeTable*> compilerTable;
    int numberOfBuckets;
    int baseScopeID;
public:
    SymbolTable(int numberOfBuckets);
    ~SymbolTable();
    void printCurrent(){if(!compilerTable.empty())compilerTable.top()->Print();}
    void printAll(){
        if(compilerTable.empty())
            return;
        ScopeTable* current=compilerTable.top();
        while(current!=NULL){
            current->Print();
            current=current->getParentScopeTable();
        }
        cout<<"ok"<<endl;
        
    }
    void EnterScope();

    void ExitScope();
    bool Insert(SymbolInfo *symbol){
        if(compilerTable.empty())return false;
        ScopeTable* current=compilerTable.top();
        return current->Insert(symbol);
    }
    bool Remove(string Name){
        if(compilerTable.empty())return false;
        return compilerTable.top()->Delete(Name);
    }
    SymbolInfo* LookUp(string Name);
};