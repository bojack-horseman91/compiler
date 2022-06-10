%option noyywrap 

%x StringLiterals
%x SingleComment
%x MultilineComment
%x CharLiterals
%x CharLiteralsPhase2

%{
#include<bits/stdc++.h>    
#include "requestProcessor.cpp"

using namespace std;

int line_count=1,error_count=0;
string stringInput="";
string comment="";
char charInput;
ofstream token;
ofstream logfile;
requestProcessor* RP;


string unescape(const string& s)
{
  string res;
  string::const_iterator it = s.begin();
  while (it != s.end())
  {
    char c = *it++;
    if (c == '\\' && it != s.end())
    {
      switch (*it++) {
      case '\\': c = '\\'; break;
      case 'n': c = '\n'; break;
      case 't': c = '\t'; break;
      // all other escapes
      case 'a': c='\a';break;
      case 'f':c='\f'; break;
      case 'r' :c='\r';break;
      case 'v' :c='\v';break;
      case '0' :c='\0';break;
      case 'b' :c='\b';break;
      case '\'' :c='\'';break;
      case '\"' :c='\"';break;
      default: 
        // invalid escape sequence - skip it. alternatively you can copy it as is, throw an exception...
        continue;
      }
    }
    res += c;
  }
 
  return res;
}
int unescape2(const string& s)
{
  string res;
  string::const_iterator it = s.begin();
  while (it != s.end())
  {
    char c = *it++;
    if (c == '\\' && it != s.end())
    {
      switch (*it++) {
      case '\\': c='\\'; break;
      case 'n': c = '\n'; break;
      case 't': c = '\t'; break;
      // "\\\\"|"\\'"|"\\a"|"\\f"|"\\r"|"\\b"|"\\v"|"\\0"
      case 'a': c='\a';break;
      case 'f':c='\f'; break;
      case 'r' :c='\r';break;
      case 'v' :c='\v';break;
      case '0' :c='\0';break;
      case 'b' :c='\b';break;
      case '\'' :c='\'';break;

      default: 
        // invalid escape sequence - skip it. alternatively you can copy it as is, throw an exception...
        continue;
      }
    }
    return c;
  }
 
  return 0;
}
%}

DIGIT [0-9]
ALPHA [a-zA-Z]
WHITESPACE [ \t\f\r\v]+ 
NEWLINE \r\n
ADDOP [+-]
MULOP "*"|"%"|"/"
INCOP "++"|"--"
RELOP "<"|"<="|">"|">="|"=="|"!="
logfileICOP "&&"|"||"
NOT	"!"
LPAREN "\("
ASSIGNOP =
RPAREN "\)"

LTHIRD "\["
RTHIRD "\]"
COMMA  ,
SEMICOLON ;
KEYWORD if|char|else|default|for|do|int|float|void|switch|while|break|double|return|case|continue
UNDERSCORE _
IDENTIFIERS (({ALPHA}|{UNDERSCORE})({UNDERSCORE}|{DIGIT}|{ALPHA})*)
ERROR_IDENTIFIERS ({DIGIT}+({UNDERSCORE}|{ALPHA})+)

ESCAPE_STR_SEQ   "\\n"|"\\t"|"\\\\"|"\\'"|"\\a"|"\\f"|"\\r"|"\\b"|"\\v"|"\\0"
FLOAT [0-9]*[.][0-9]+(E(-)?[0-9]+)?
ILL_FORMED_FLOAT {FLOAT}([.][0-9]+(E(-)?[0-9]+)?)+

%%
{NEWLINE} {line_count++;}
\n {line_count++;}
{WHITESPACE} {}
\/\/ {BEGIN SingleComment;comment="";}
<SingleComment>.* {comment=yytext;logfile<<line_count<<"<SINGLELINE_COMMENT,"<<yytext<<"> "<<endl; }
<SingleComment>\n {BEGIN INITIAL;line_count++;}
\/\* {BEGIN MultilineComment;comment="";}
<MultilineComment><<EOF>>  { logfile<<line_count<<"Error at line number :"<<line_count<<"Unfinished comment UNEXPECTED ENDING"<<endl;error_count++;BEGIN INITIAL ;}
<MultilineComment>\*\/ {BEGIN INITIAL;logfile<<line_count<<"<MULTILINE_COMMENT,"<<comment<<"> "<<endl; }
<MultilineComment>{NEWLINE} {line_count++;}
<MultilineComment>([^(\*\/)(\r\n)])* {comment+=yytext;}
{FLOAT} {token<<"< CONST_FLOAT,"<<yytext<<"> ";
        string cmd="I ";
                      cmd+=yytext;
                      cmd+=" FLOAT";
                      logfile<<line_count<<"< CONST_FLOAT,"<<yytext<<"> "<<endl;
                      RP->processRequest(cmd,logfile);
        }
{ILL_FORMED_FLOAT} {logfile<<"Error at line number :"<<line_count<<"Too many decimal point error or Ill formed number"<<endl;error_count++;}
[0-9]+ {token<<"< CONST_INT,"<<yytext<<"> ";
        string cmd="I ";
                      cmd+=yytext;
                      cmd+=" INT";
                      RP->processRequest(cmd,logfile);
        logfile<<line_count<<"< CONST_INT,"<<yytext<<"> "<<endl;
        }
<CharLiterals>([^\']|{ESCAPE_STR_SEQ}) {
						string s=yytext;
						if(yyleng>1){
							charInput=unescape2(s.substr(0,2));
							
							}
						else
							charInput=s[0];
            BEGIN CharLiteralsPhase2;
              
					}
' {BEGIN CharLiterals;}
<CharLiterals>\' {logfile<<"Error at line number :"<<line_count<<"Empty character constant error"<<endl;error_count++;BEGIN INITIAL;}
<CharLiteralsPhase2>\' {token<<"<CONST_CHAR,"<<charInput<<">"<<endl;
                        logfile<<line_count<<"<CONST_CHAR,"<<charInput<<">"<<endl;
                        string cmd="I ";
                          cmd+=charInput;
                          cmd+=" CONST_CHAR";
                          RP->processRequest(cmd,logfile);
                        BEGIN INITIAL;}
<CharLiteralsPhase2>({NEWLINE}) {
                                      logfile<<"Error at line number :"<<line_count<<"Unfinished character"<<endl;error_count++;
                                      line_count++;
                                      BEGIN INITIAL;
                                    }
<CharLiteralsPhase2>[^\'" "(\r\n)]*(\')? {BEGIN INITIAL;
                                        logfile<<"Error at line number :"<<line_count<<"Multi character constant error for character sequence "<<endl;
                                        error_count++;}
{KEYWORD} {		
				string input=yytext;
				transform(input.begin(), input.end(), input.begin(), ::toupper);
			  token<<"<"<<input<<">";
        logfile<<line_count<<"<"<<input<<">"<<endl;}
{IDENTIFIERS} {
        token<<" <ID,"<<yytext<<">";
        string cmd="I ";
                      cmd+=yytext;
                      cmd+=" ID";
                      logfile<<line_count<<" <ID,"<<yytext<<">"<<endl;
                      RP->processRequest(cmd,logfile);
				
			}
{INCOP} {token<<" <INCOP,"<<yytext<<">";
          logfile<<line_count<<" <INCOP,"<<yytext<<">"<<endl;}
{ADDOP} {token<<" <ADDOP,"<<yytext<<">";
          logfile<<line_count<<" <ADDOP,"<<yytext<<">"<<endl;}
{LPAREN} {token<<" <LPAREN,"<<yytext<<">"<<endl;
          logfile<<line_count<<" <LPAREN,"<<yytext<<">"<<endl;}
{MULOP} {token<<" <MULOP,"<<yytext<<">"<<endl;
        logfile<<line_count<<" <MULOP,"<<yytext<<">"<<endl;}
{RELOP} {token<<" <RELOP,"<<yytext<<">"<<endl;
        logfile<<line_count<<" <RELOP,"<<yytext<<">"<<endl;}
{ASSIGNOP} {token<<" <ASSIGNOP,"<<yytext<<">"<<endl;
            logfile<<line_count<<" <ASSIGNOP,"<<yytext<<">"<<endl;}
{logfileICOP} {token<<" <logfileICOP,"<<yytext<<">"<<endl;
            logfile<<line_count<<" <logfileICOP,"<<yytext<<">"<<endl;}
{NOT} {token<<" <NOT,"<<yytext<<">"<<endl;
      logfile<<line_count<<" <NOT,"<<yytext<<">"<<endl;}
{RPAREN} {token<<" <RPAREN,"<<yytext<<">"<<endl;
        logfile<<line_count<<" <RPAREN,"<<yytext<<">"<<endl;}
"{" {token<<" <LCURL,"<<yytext<<">"<<endl;
      logfile<<line_count<<" <LCURL,"<<yytext<<">"<<endl;
      string cmd="S";
                      
                      RP->processRequest(cmd,logfile);}
"}" {token<<" <CURL,"<<yytext<<">"<<endl;
    logfile<<line_count<<" <CURL,"<<yytext<<">"<<endl;
    string cmd="E";

                      RP->processRequest(cmd,logfile);
    }
{LTHIRD} {token<<" <LTHIRD,"<<yytext<<">"<<endl;
          logfile<<line_count<<" <LTHIRD,"<<yytext<<">"<<endl;}
{RTHIRD} {token<<" <RTHIRD,"<<yytext<<">"<<endl;
          logfile<<line_count<<" <RTHIRD,"<<yytext<<">"<<endl;}
{COMMA} {token<<" <COMMA,"<<yytext<<">"<<endl;
        logfile<<line_count<<" <COMMA,"<<yytext<<">"<<endl;}
{SEMICOLON} {token<<" <SEMICOLON,"<<yytext<<">"<<endl;
            logfile<<line_count<<" <SEMICOLON,"<<yytext<<">"<<endl;}

\" {BEGIN(StringLiterals);stringInput="";}
<StringLiterals>\"   { token<<"<STRING ,"<<stringInput<<"> "<<endl;
                      // string cmd="I ";
                      // cmd+=stringInput;
                      // cmd+=" STRING";
                      // RP->processRequest(cmd,logfile);
                      logfile<<line_count<<"<STRING ,"<<stringInput<<"> "<<endl;
                       
                       BEGIN INITIAL ;}
<StringLiterals>\\\r\n {line_count++;}
<StringLiterals>\n {logfile<<line_count<<"Error at line number :"<<line_count<<"Unfinished string"<<endl;error_count++;line_count++;BEGIN INITIAL;}
<StringLiterals><<EOF>> {logfile<<line_count<<"Error at line number :"<<line_count<<"Unfinished string UNEXPECTED ENDING"<<endl;error_count++;BEGIN INITIAL;}
<StringLiterals>([^\n\\\"]|(\\[^\r]))* {stringInput+=unescape(yytext);}

{ERROR_IDENTIFIERS} {logfile<<line_count<<"Error at line number :"<<line_count<<"Invalid Suffix on numeric constant or invalid prefix"<<endl;error_count++;}
. {logfile<<line_count<<"Error at line number :"<<line_count<<" Unrecognized character"<<endl;error_count++;}
%%
int main(int argc,char *argv[]){
  token.open ("1805035_token.txt");
  logfile.open("1805035_logfile.txt");
  RP=new requestProcessor(10);
  if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	

	yyin= fin;
	yylex();
	fclose(yyin);
  token.close();
  logfile.close();
  delete RP;
	return 0;
}
