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
            //current=current->getNext();
        }
    }
    void EnterScope(){
    if(compilerTable.empty()){
        compilerTable.push(new ScopeTable(numberOfBuckets,to_string(baseScopeID++)+"."));
        return;
    }
    string  nextID=compilerTable.top()->getNextScopeID();
    ScopeTable* newTable=new ScopeTable(numberOfBuckets,nextID);
    newTable->setParentScopeTable(compilerTable.top());
    compilerTable.push(newTable);
    }
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



SymbolInfo* SymbolTable::LookUp(string Name){
    if(compilerTable.empty()){
      return NULL;
    }
    ScopeTable* current=compilerTable.top();
    do{
            SymbolInfo* symbol=current->LookUp(Name);
        if(symbol!=NULL)
        {
            return symbol;
        }
        current=current->getParentScopeTable();
    }while(current!=NULL);
    return NULL;
}
void SymbolTable::ExitScope(){
    compilerTable.pop();
}

SymbolTable::SymbolTable(int numberOfBuckets)
{
    this->numberOfBuckets=numberOfBuckets;
    baseScopeID=1;
}

SymbolTable::~SymbolTable()
{
}
int main(){

    SymbolInfo* si=new SymbolInfo("a","a");
    SymbolInfo* si2=new SymbolInfo("a2","a2");
    SymbolInfo* si3=new SymbolInfo("a22","a22");
    SymbolTable *st=new SymbolTable(7);
    st->EnterScope();
    st->Insert(si);
    st->printCurrent();
    st->EnterScope();
    st->Insert(si2);
    st->printCurrent();
    st->Insert(si3);
    st->printCurrent();
    st->ExitScope();
    
    st->EnterScope();
    st->Insert(si3);
    st->printCurrent();

}
