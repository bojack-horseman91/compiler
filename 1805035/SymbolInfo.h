#ifndef SYMBOLINFO_H_INCLUDED
#define SYMBOLINFO_H_INCLUDED
#include<string>
using namespace std;
class SymbolInfo
{
private:
        string  Name;
        string Type;
        string ScopeTableID;
        SymbolInfo*Next;
        int row,col;
       
public:
        SymbolInfo();
        SymbolInfo(string Name,string Type);
        ~SymbolInfo();
        bool operator == (SymbolInfo other){
            return (other.getName()==this->Name)&& (other.getType()==this->Type);
        }
        void setRow(int x){row=x;}
        void setCol(int y){col=y;}
        void setScopeTabelID(string s){ScopeTableID=s;}
        string getScopeTableID(){return ScopeTableID;}
        int getRow(){return row;}
        int getCol(){return col;}
        void print(){
            cout<<"< "<<Name<<" : "<<Type<<">  ";
            if(Next!=NULL){cout<<"-";Next->print();}
        }
        void print(ofstream&logWritter){
            logWritter<<"< "<<Name<<" : "<<Type<<">  ";
            if(Next!=NULL){logWritter<<"-";Next->print(logWritter);}
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
