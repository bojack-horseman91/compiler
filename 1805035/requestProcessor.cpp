#include "SymbolTable.h"
#include <bits/stdc++.h>

using namespace std;

class request{
    private:
    string command;
    vector<string>others;
    public:
    string getCommand(){return command;}
    string getOthersAt(int i){return others[i];}
    void setCommand(string command){this->command=command;}
    void addOthers(string o){others.push_back(o);}
    void print(){cout<<command<<" ";for(string s:others)cout<<s<<" ";cout<<endl;}
   

};

class requestProcessor{

    private:
    SymbolTable * st;
   
    request* tokenizer(string line){
        vector <string> tokens;
        
        // stringstream class check1
        stringstream check1(line);
        
        string intermediate;
        getline(check1, intermediate, ' ');
        request* r=new request();
        r->setCommand(intermediate);
        // Tokenizing w.r.t. space ' '
        while(getline(check1, intermediate, ' '))
        {
            r->addOthers(intermediate);
        }
        
        
        
        return r;
    }
    public:
    
    requestProcessor(int n){
        
        st=new SymbolTable(n);
        st->EnterScope();
    }
    ~requestProcessor(){
        delete st;
    }
    void processRequest(string line,ofstream&logWritter){
        
        request* requests=tokenizer(line);
        requests->print();
        string command=requests->getCommand();
        // cout<<"**************"<<command<<endl;
        if(command=="I"){
            // cout<<"Start"<<endl;
            // cout<<requests->getOthersAt(0)<<requests->getOthersAt(1)<<endl;
            
            SymbolInfo* symbol=new SymbolInfo(requests->getOthersAt(0),requests->getOthersAt(1));
            
            if(st->Insert(symbol)){
                logWritter<<"*******************"<<endl;
                logWritter<<"Inserted in ScopeTable# "<<symbol->getScopeTableID()<<" at position "<<symbol->getRow()<<", "<<symbol->getCol()<<endl;
                st->printAllLex(logWritter);
                logWritter<<"*******************"<<endl;
            }
            else{
               symbol->print(logWritter); logWritter<<"Already Exists"<<endl;
            }
               
        }
        else if(command=="S"){
            st->EnterScope();
        }
        else if (command=="P")
        {
            if(requests->getOthersAt(0)=="A")
                st->printAll();
            else
                st->printCurrent();
            // cout<<"ok2"<<endl;
        }
        else if (command=="L")
        {
            st->LookUp(requests->getOthersAt(0));
        }
        else if (command=="D")
        {
            
            st->Remove(requests->getOthersAt(0));
        }
        else if (command=="E")
        {
            st->ExitScope();
        }
        cout<<"*******************"<<endl;
        
        
    }
};
// std::istream& safeGetline(std::istream& is, std::string& t)
// {
//     t.clear();

//     // The characters in the stream are read one-by-one using a std::streambuf.
//     // That is faster than reading them one-by-one using the std::istream.
//     // Code that uses streambuf this way must be guarded by a sentry object.
//     // The sentry object performs various tasks,
//     // such as thread synchronization and updating the stream state.

//     std::istream::sentry se(is, true);
//     std::streambuf* sb = is.rdbuf();

//     for(;;) {
//         int c = sb->sbumpc();
//         switch (c) {
//         case '\n':
//             return is;
//         case '\r':
//             if(sb->sgetc() == '\n')
//                 sb->sbumpc();
//             return is;
//         case std::streambuf::traits_type::eof():
//             // Also handle the case when the last line has no line ending
//             if(t.empty())
//                 is.setstate(std::ios::eofbit);
//             return is;
//         default:
//             t += (char)c;
//         }
//     }
// }
// int main(){
//     string myText;

//     // Read from the text file
//     ifstream MyReadFile("input.txt");
//     getline (MyReadFile, myText);
//     requestProcessor* RP=new requestProcessor(stoi(myText));


//     // Use a while loop together with the getline() function to read the file line by line
//     while (safeGetline (MyReadFile, myText)) {
//     // Output the text from the file
//         RP->processRequest(myText);
//     }

//     // Close the file
//     MyReadFile.close();
    
//     delete RP;
// //     SymbolTable* st=new SymbolTable(7);
// //     st->EnterScope();
// //     SymbolInfo* symbol=new SymbolInfo("a","a");
// //     st->Insert(symbol);
// //     st->EnterScope();
// //    symbol=new SymbolInfo("h","h");
// //     st->Insert(symbol);
// //     st->EnterScope();
// //     symbol=new SymbolInfo("o","o");
// //     st->Insert(symbol);
// //     st->printCurrent();
// //     st->LookUp("o");

//     return 0;
// }