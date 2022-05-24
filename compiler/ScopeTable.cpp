#include <bits/stdc++.h>
#include "SymbolInfo.h"
#include"ScopeTable.h"
using namespace std;


ScopeTable::ScopeTable(int NumberOfBucket ,string ScopeID)
{
    this->parentScopeTable=NULL;
    this->ScopeID=ScopeID;
    hashTable=new SymbolInfo*[NumberOfBucket];
    numberOfBucket=NumberOfBucket;
    for(int i=0;i<numberOfBucket;i++){
        hashTable[i]=NULL;
    }
}
static unsigned long
 sdbm(unsigned char *str)

    {
        unsigned long hash = 0;
        int c;

        while (c = *str++)
            hash = c + (hash << 6) + (hash << 16) - hash;
        cout<<"hash value:"<<hash<<endl;
        return hash;
    }

int ScopeTable::hashf(string Name){
    unsigned char trap[Name.length()+1];

    copy( Name.begin(), Name.end(), trap );
    trap[Name.length()] = 0;
    return  sdbm(trap)% numberOfBucket;
}



bool ScopeTable::Insert(SymbolInfo* symbol){
    int bucketNumber=this->hashf(symbol->getName());
    SymbolInfo* current=hashTable[bucketNumber];
    SymbolInfo* prev;
    SymbolInfo* head=current;
    int position=0;
    //cout<<bucketNumber<<endl;
    //cout<<head<<current<<hashTable[bucketNumber]<<endl;
    if(current==NULL){
        printInsertSuccess(bucketNumber,position);
        current=symbol;
        hashTable[bucketNumber]=head=current;

        return true;
    }
    while(current!=NULL){
        cout<<current->getName()<<endl;
        if(*current==*symbol){
            symbol->print();cout<<" already exists in current ScopeTable"<<endl;
            return false;
        }
        prev=current;
        current=current->getNext();
        position++;
    }
    printInsertSuccess(bucketNumber,position);
    prev->setNext(symbol);
    hashTable[bucketNumber]=head;
    return true;

}
void ScopeTable::Print(){
    cout<<"ScopeTable # "<<this->getID()<<endl;
    for(int i=0;i<numberOfBucket;i++){
        cout<<i<<" : ";
        if(hashTable[i]!=NULL){
            hashTable[i]->print();
        }
        cout<<endl;
    }
}
ScopeTable::~ScopeTable()
{
    delete[] hashTable;
    
}

SymbolInfo* ScopeTable::LookUp(string Name ){
    int bucketNumber=this->hashf(Name);
    SymbolInfo* current=hashTable[bucketNumber];
    int position=0;
    if(current==NULL){
        cout<<"Not found"<<Name<<endl;
        return NULL;
    }
    while(current!=NULL){

        if(current->getName()==Name){
            cout<<"Found in ScopeTable# "<<ScopeID<<" at position "<<bucketNumber<<", "<<position;
            return current;
        }
        current=current->getNext();
        position++;
    }
    cout<<"Not found"<<Name<<endl;
    return NULL;
}

bool ScopeTable::Delete(string Name){
    int bucketNumber=this->hashf(Name);
    SymbolInfo* current=hashTable[bucketNumber];
    SymbolInfo* prev=NULL;
    SymbolInfo* head=current;
    int position=0;
    //cout<<bucketNumber<<endl;
    //cout<<head<<current<<hashTable[bucketNumber]<<endl;
    if(current==NULL){
        cout<<"Not found"<<Name<<endl;
        return false;
    }
    while(current!=NULL){

        if(current->getName()==Name){
            if(prev!=NULL){
                prev->setNext(current->getNext());

            }
            else
                hashTable[bucketNumber]=NULL;
            current->~SymbolInfo();
            cout<<"Deleted Entry "<<bucketNumber<<", "<<position<<" from current ScopeTable"<<endl;
            return true;
        }

        current=current->getNext();
        position++;
    }
    cout<<"Not found"<<Name<<endl;
    return false;
}

