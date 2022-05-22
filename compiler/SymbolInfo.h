#ifndef SYMBOLINFO_H_INCLUDED
#define SYMBOLINFO_H_INCLUDED
#include<string>
using namespace std;
class SymbolInfo
{
private:
        string  Name;
        string Type;
        SymbolInfo*Next;

public:
        SymbolInfo();
        SymbolInfo(string Name,string Type);
        ~SymbolInfo();
        bool operator == (SymbolInfo other){
            return (other.getName()==this->Name)&& (other.getType()==this->Type);
        }

        void print(){
            cout<<"< "<<Name<<" : "<<Type<<">  ";
            if(Next!=NULL){cout<<"-";Next->print();}
        }
        //getters and setters
        //name
        void setName(string Name){this->Name=Name;}
        string getName() {return Name;}
        //type
        void setType(string Type){this->Type=Type;}
        string getType(){return Type;}
        //next
        void setNext(SymbolInfo* Next){this->Next=Next;}
        SymbolInfo* getNext(){return Next;}



};

#endif // SYMBOLINFO_H_INCLUDED
