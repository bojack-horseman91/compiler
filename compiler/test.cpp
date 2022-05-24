#include <bits/stdc++.h>

using namespace std;

static unsigned int SDBMHash(string str) {
	unsigned int hash = 0;
	unsigned int i = 0;
	unsigned int len = str.length();

	for (i = 0; i < len; i++)
	{
		hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
	}
    cout<<hash<<endl;

	return hash;
}

int main(){
    string f="foo";
    SDBMHash(f);


}